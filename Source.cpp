#include <iostream>
#include <vector>

#include <chrono>

#include <thread>
#include <mutex>

#include <random>

#ifdef _MSC_VER
#include <Windows.h>
#define CLR_STR "cls"
#define DEBUG_BREAK __debugbreak()
#else // linux
#error 'GetAsyncKeyState' alternative not implemented!
#include <unistd.h>
#define CLR_STR "clear"
#define DEBUG_BREAK raise(SIGTRAP)
#endif

#define SNAKE_HEAD '@'
#define SNAKE_BODY '#'
#define FRUIT	   'O'
#define WIDTH       20
#define HEIGHT	    20

static bool s_Running = true;
static std::mutex s_InputMutex;

struct Vec2
{
	float x, y;
	Vec2() : x(0.f), y(0.f) {}
	Vec2(float x, float y) : x(x), y(y) {}

	Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }

	Vec2 operator-(const Vec2& other) const	{ return Vec2(x - other.x, y - other.y); }

	Vec2 operator-() const { return Vec2(-x, -y); }

	Vec2 operator*(const Vec2& other) const { return Vec2(x * other.x, y * other.y); }

	Vec2 operator/(const Vec2& other) const { return Vec2(x / other.x, y / other.y); }

	Vec2 operator*(float a) const { return Vec2(x * a, y * a); }

	Vec2 operator/(float a) const { return Vec2(x / a, y / a); }

	Vec2& operator+=(const Vec2& other) 
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vec2& operator-=(const Vec2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vec2& operator*=(const Vec2& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	Vec2& operator*=(float a)
	{
		x *= a;
		y *= a;
		return *this;
	}

	Vec2& operator/=(const Vec2& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	Vec2& operator/=(float a)
	{
		x /= a;
		y /= a;
		return *this;
	}

	bool operator==(const Vec2& other) const { return x == other.x && y == other.y; }
};

struct Snake
{
	std::vector<Vec2> parts;
	Vec2 Direction;

	Snake(const Vec2& position = Vec2(9.f, 9.f), const Vec2& direction = Vec2(1.f, 0.f))
		: Direction(direction)
	{
		// initial size = 2 (1 head, 1 tail)
		parts.push_back(position + direction);
		parts.push_back(position);
	}

	const Vec2& Head() const { return parts[0]; }

	void Grow()
	{
		// set new part to be same as tail
		// it will be changed when the snake moves
		parts.push_back(parts.back());
	}

	bool ChangeDir(const Vec2& dir)
	{
		// if player tries to reverse direction
		// ignore it
		if (parts[0] + dir == parts[1])
			return false;

		Direction = dir;
		return true;
	}

	void Move(float x_max = WIDTH, float y_max = HEIGHT)
	{
		// shift all parts 1 step back (1 = 0, 2 = 1, 3 = 2, .... )
		std::memmove(&parts[1], &parts[0], (parts.size() - 1) * sizeof(Vec2));
		// move the first part (head) by direction
		auto& part = (parts[0] += Direction);

		// loop back around if head goes out of map
		if (part.x >= WIDTH)
			part.x = 0.f;
		else if (part.x < 0.f)
			part.x = WIDTH - 1.f;

		if (part.y >= HEIGHT)
			part.y = 0.f;
		else if (part.y < 0.f)
			part.y = HEIGHT - 1.f;
	}
};

// used to decide fruit location
static std::random_device rd;
static std::mt19937 engine(rd());
static std::uniform_int_distribution<int32_t> dist(0, 19);

struct Level
{
	Snake player;
	Vec2 fruit;

	Level()
	{
		// set a random location for fruit on startup
		MoveFruit();

	}

	void MoveFruit()
	{
		// check if fruit is on a valid tile ( not on a tile with snake part )
		std::vector<Vec2> tiles;
		for (size_t i = 0; i < HEIGHT; i++)
			for (size_t j = 0; j < WIDTH; j++)
				tiles.push_back(Vec2((float)i, (float)j));

		// remove all tiles with snake parts
		for (auto& part : player.parts)
		{
			auto it = std::find(tiles.begin(), tiles.end(), part);
			if (it != tiles.end())
				tiles.erase(it);
		}

		// if there are no valid tiles
		// player won ?
		if (tiles.empty())
		{
			std::cout << "Game Won...." << std::endl << std::endl;
			// acquire lock because s_Running is used by input_thread
			std::scoped_lock<std::mutex> lock(s_InputMutex);
			s_Running = false;
		}
		else
		{
			// choose a random tile from the valid tiles
			std::uniform_int_distribution<int32_t> idx(0, (int32_t)tiles.size() - 1);
			int32_t index = idx(engine);
			fruit = tiles[index];
		}
	}

	void Update()
	{
		player.Move();
		// if player ate fruit
		if (player.Head() == fruit)
		{
			player.Grow();
			MoveFruit();
		}
		else
		{
			// check if player ran into snake part
			for (auto it = player.parts.begin() + 1; it != player.parts.end(); ++it)
			{
				auto& part = *it;
				if (part == player.Head())
				{
					std::cout << "Game Over...." << std::endl << std::endl;
					// acquire lock because s_Running is used by input_thread
					std::scoped_lock<std::mutex> lock(s_InputMutex);
					s_Running = false;
				}
			}
		}
	}
};

struct Screen
{
	char** scr = nullptr;
	uint32_t width, height;
	Screen(uint32_t width = WIDTH, uint32_t height = HEIGHT)
		: width(width), height(height)
	{
		scr = new char* [height];
		for (size_t i = 0; i < height; i++)
		{
			scr[i] = new char[width];
			memset(scr[i], 0, width);
		}
	}

	~Screen()
	{
		for (size_t i = 0; i < height; i++)
			delete[] scr[i];

		delete[] scr;
	}

	void Clear(char cl = 0)
	{
		for (size_t i = 0; i < height; i++)
			memset(scr[i], cl, width);
	}

	void Print()
	{
		for (size_t h = 0; h < height; h++)
		{
			for (size_t w = 0; w < width; w++)
				std::cout << scr[h][w] << " ";

			std::cout << std::endl;
		}
	}

	void Draw(const Snake& snake)
	{
		if (snake.parts.empty())
			return;


		for (auto it = snake.parts.begin() + 1; it != snake.parts.end(); ++it)
		{
			auto& part = *it;
			scr[(size_t)part.y][(size_t)part.x] = SNAKE_BODY;
		}
		// draw head last so it shows up when player runs into snake
		scr[(size_t)snake.parts[0].y][(size_t)snake.parts[0].x] = SNAKE_HEAD;
	}

	void Draw(const Vec2& fruit)
	{
		scr[(size_t)fruit.y][(size_t)fruit.x] = FRUIT;
	}

	void Draw(const Level& level)
	{
		Draw(level.player);
		Draw(level.fruit);
	}
};

static char s_InputData;
static Level s_Level;
static Screen s_Screen;

void poll_events()
{
	// acquire lock because 's_InputData' is updated by a different thread
	std::scoped_lock<std::mutex> lock(s_InputMutex);

	if (!s_InputData)
		return;

	// controls the lifetime of application and input thread
	s_Running = (s_InputData != 'q');
	switch (s_InputData)
	{
	case 'w':
		s_Level.player.ChangeDir(Vec2(0.f, -1.f));
		break;
	case 'a':
		s_Level.player.ChangeDir(Vec2(-1.f, 0.f));
		break;
	case 's':
		s_Level.player.ChangeDir(Vec2(0.f, 1.f));
		break;
	case 'd':
		s_Level.player.ChangeDir(Vec2(1.f, 0.f));
		break;
	}

	s_InputData = 0;
}

// run on a separate thread
void T_ReadInput()
{
	std::unique_lock<std::mutex> lock(s_InputMutex);
	while (s_Running)
	{
		// release mutex when we are checking for input
		lock.unlock();
		char ch = 0;
		if (GetAsyncKeyState(VK_UP))
			ch = 'w';
		else if (GetAsyncKeyState(VK_DOWN))
			ch = 's';
		else if (GetAsyncKeyState(VK_LEFT))
			ch = 'a';
		else if (GetAsyncKeyState(VK_RIGHT))
			ch = 'd';
		else if (GetAsyncKeyState(VK_ESCAPE))
			ch = 'q';

		// re acquire lock when we setting input and also checking 's_Running' for next loop
		lock.lock();
		if (ch)
			s_InputData = ch;
	}
}

// called once every second
void update()
{
	system(CLR_STR);
	s_Screen.Clear('*');
	s_Level.Update();
	s_Screen.Draw(s_Level);
	s_Screen.Print();

	std::cout << std::endl << "Score: " << s_Level.player.parts.size() - 1 << std::endl;
}

int main()
{
	// separate input reading into separate thread 
	// so that we can constantly read input
	// instead of checking input every 1 second
	std::thread input_thread(T_ReadInput);

	while (s_Running)
	{
		// check if there was any input and
		// react to it
		poll_events();
		update();

		using namespace std::chrono_literals;
		std::this_thread::sleep_for(1000ms);
	}

	input_thread.join();
	std::cout << std::endl << "Press any key to quit" << std::endl;
	std::cin.get();
	return 0;
}