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

struct InputData
{
	char LastInput = 0;

	InputData(char ch = 0) : LastInput(ch) {}

	operator bool() const
	{
		return LastInput;
	}

	operator char() const
	{
		return LastInput;
	}
};

struct Vec2
{
	float x, y;
	Vec2() : x(0.f), y(0.f) {}
	Vec2(float x, float y) : x(x), y(y) {}

	Vec2 operator+(const Vec2& other) const
	{
		return Vec2(x + other.x, y + other.y);
	}

	Vec2 operator-(const Vec2& other) const
	{
		return Vec2(x - other.x, y - other.y);
	}

	Vec2 operator-() const
	{
		return Vec2(-x, -y);
	}

	Vec2 operator*(const Vec2& other) const
	{
		return Vec2(x * other.x, y * other.y);
	}

	Vec2 operator/(const Vec2& other) const
	{
		return Vec2(x / other.x, y / other.y);
	}

	Vec2 operator*(float a) const
	{
		return Vec2(x * a, y * a);
	}

	Vec2 operator/(float a) const
	{
		return Vec2(x / a, y / a);
	}

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

	bool operator==(const Vec2& other) const
	{
		return x == other.x && y == other.y;
	}
};

class Timer
{
public:
	Timer()
	{
		Reset();
	}

	void Reset()
	{
		mStart = std::chrono::high_resolution_clock::now();
	}

	float Elapsed()
	{
		return (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - mStart).count() * 1e-9);
	}

	float ElapsedMilli()
	{

		return (float)(Elapsed() * 1e3);
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> mStart;
};

class Time
{
public:
	static float GetTime() { return sAppTimer.Elapsed(); }
	static float GetTimeMilli() { return sAppTimer.ElapsedMilli(); }
	static void Reset() { sAppTimer.Reset(); }

private:
	static inline Timer sAppTimer;
};

struct Snake
{
	std::vector<Vec2> parts;
	Vec2 Direction;

	Snake(const Vec2& position = Vec2(9.f, 9.f), const Vec2& direction = Vec2(1.f, 0.f))
		: Direction(direction)
	{
		parts.push_back(position + direction);
		parts.push_back(position);
	}

	const Vec2& Head() const { return parts[0]; }

	void Grow()
	{
		parts.push_back(parts.back());
	}

	bool ChangeDir(const Vec2& dir)
	{
		if (parts[0] + dir == parts[1])
			return false;
		Direction = dir;
		return true;
	}

	void Move(float x_max = WIDTH, float y_max = HEIGHT)
	{
		std::memmove(&parts[1], &parts[0], (parts.size() - 1) * sizeof(Vec2));
		auto& part = (parts[0] += Direction);

		if (part.x >= WIDTH)
			part.x = 0.f;
		else if (part.x < 0.f)
			part.x = WIDTH - 1.f;

		if (part.y >= HEIGHT)
			part.y = 0.f;
		else if (part.y < 0.f)
			part.y = HEIGHT - 1.f;
	}
	
	size_t size() const
	{
		return parts.size();
	}
};

static std::random_device rd;
static std::mt19937 engine(rd());
static std::uniform_int_distribution<int32_t> dist(0, 19);

struct Level
{
	Snake player;
	Vec2 fruit;

	Level()
	{
		MoveFruit();

	}

	void MoveFruit()
	{
		fruit = Vec2((float)dist(engine), (float)dist(engine));
	}

	void Update()
	{
		player.Move();
		if (player.Head() == fruit)
		{
			player.Grow();
			MoveFruit();
			std::vector<Vec2> tiles;
			for (size_t i = 0; i < HEIGHT; i++)
				for (size_t j = 0; j < WIDTH; j++)
					tiles.push_back(Vec2((float)i, (float)j));

			for (auto& part : player.parts)
			{
				auto it = std::find(tiles.begin(), tiles.end(), part);
				if (it != tiles.end())
					tiles.erase(it);
			}

			auto it = std::find(tiles.begin(), tiles.end(), fruit);
			if (it == tiles.end())
			{
				std::uniform_int_distribution<int32_t> idx(0, (int32_t)tiles.size() - 1);
				int32_t index = idx(engine);
				fruit = tiles[index];
			}
		}
		else
		{
			for (auto it = player.parts.begin() + 1; it != player.parts.end(); ++it)
			{
				auto& part = *it;
				if (part == player.Head())
				{
					std::cout << "Game Over...." << std::endl << std::endl;
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

		scr[(size_t)snake.parts[0].y][(size_t)snake.parts[0].x] = SNAKE_HEAD;

		for (auto it = snake.parts.begin() + 1; it != snake.parts.end(); ++it)
		{
			auto& part = *it;
#ifndef NDEBUG
			if (part.x > width || part.x < 0 || part.y > height || part.y < 0)
				DEBUG_BREAK;
#endif
			scr[(size_t)part.y][(size_t)part.x] = SNAKE_BODY;
		}
	}

	void Draw(const Vec2& fruit)
	{
		scr[(size_t)fruit.y][(size_t)fruit.x] = FRUIT;
	}
};

static InputData s_InputData;
static Level s_Level;
static Screen s_Screen;

void poll_events()
{
	std::scoped_lock<std::mutex> lock(s_InputMutex);

	if (!s_InputData)
		return;

	s_Running = (s_InputData.LastInput != 'q');
	switch (s_InputData.LastInput)
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

void T_ReadInput()
{
	std::unique_lock<std::mutex> lock(s_InputMutex);
	while (s_Running)
	{
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
		lock.lock();
		s_InputData.LastInput = ch;
	}
}

void update()
{
	system(CLR_STR);
	s_Screen.Clear('*');
	s_Level.Update();
	s_Screen.Draw(s_Level.player);
	s_Screen.Draw(s_Level.fruit);
	s_Screen.Print();
	std::cout << std::endl << "Score: " << s_Level.player.parts.size() - 1 << std::endl;
}

int main()
{
	std::thread input_thread(T_ReadInput);

	float last_time = Time::GetTime(), cur_time = Time::GetTime();
	float acc = 0.f;

	while (s_Running)
	{
		last_time = cur_time;
		cur_time = Time::GetTime();
		float delta = cur_time - last_time;
		acc += delta;

		if (acc >= 1.f)
		{
			acc = 0.f;
			update();
		}

		poll_events();
	}

	input_thread.join();
	std::cout << std::endl << "Press any key to quit" << std::endl;
	std::cin.get();
	return 0;
}