#include <functional>

#include "../core/app.h"

#include "level.h"
#include "game.h"

namespace Utils
{
	static bool CheckSize(const Core::View& view, const Math::Vec2i& minSize)
	{
		return (view.GetWidth() < minSize.x || view.GetHeight() < minSize.y);
	}

	static bool InBound(const Math::Vec2i& vec, const Math::Vec2i& bound)
	{
		return (vec.x < bound.x && vec.y < bound.y);
	}
}

namespace Game
{
	MainMenu::MainMenu()
	{
	}

	MainMenu::~MainMenu()
	{
	}

	bool MainMenu::OnEvent(Core::Event e)
	{
		switch (e)
		{
		case Core::Event::Down:
			mSelectedOption = (Option)(((int)mSelectedOption + 1) % (int)Option::Invalid);
			break;
		case Core::Event::Up:
			mSelectedOption = (Option)(((int)mSelectedOption - 1) < 0 ? ((int)Option::Invalid - 1) : ((int)mSelectedOption - 1));
			break;

		case Core::Event::Enter:
		{
			if (mSelectedOption == Option::Quit)
			{
				Core::Application::Get().Close();
			}
			else
			{
				// start game
				return true;
			}

			break;
		}
		}

		return false;
	}

	void MainMenu::FillView(Core::View* pView)
	{
		// check if windows is large enough
		if (Utils::CheckSize(*pView, {18, 9}))
		{
			pView->DrawTextCentered("Window Size Too small!", Math::Vec2f(0.f, 0.f));
			return;
		}

		//border
		{

			const auto& extent = pView->GetCharExtent();
			Math::Vec2f borderPos = { -1.f, -1.f };

			auto line = [pView, &borderPos]
			(int32_t count, float inc_by, float& val)
			{
				for (int32_t i = 0; i < count; i++)
				{
					pView->DrawSprite({ {"*"} }, borderPos);
					val += inc_by;
				}
			};

			// top line
			line((int32_t)(2.f / extent.x), extent.x - std::numeric_limits<float>::epsilon(), borderPos.x);
			// right line
			line((int32_t)(2.f / extent.y), extent.y - std::numeric_limits<float>::epsilon(), borderPos.y);
			//bottome line
			line((int32_t)(2.f / extent.x), -extent.x + std::numeric_limits<float>::epsilon(), borderPos.x);
			//left line
			line((int32_t)(2.f / extent.y), -extent.y + std::numeric_limits<float>::epsilon(), borderPos.y);
		}

		// options
		{
			static Core::AnimatedSprite selectHighlightLeft =
			{
				3,
				0,
				0.f,
				{
					{
					{"-"}
					},
					{
					{">"}
					},
					{
					{"|"}
					},
					{
					{">"}
					},
				}
			};

			static Core::AnimatedSprite selectHighlightRight =
			{
				3,
				0,
				0.f,
				{
					{
					{"-"}
					},
					{
					{"<"}
					},
					{
					{"|"}
					},
					{
					{"<"}
					},
				}
			};
			// HACK: draw it out of bounds to only animate it 
			pView->DrawAnimatedSprite(selectHighlightLeft, Math::Vec2f(-2.f, -2.f));
			pView->DrawAnimatedSprite(selectHighlightRight, Math::Vec2f(-2.f, -2.f));

			std::vector<Option> options = { Option::StartGame, Option::LevelSelect, Option::Quit };
			float yInc = 1.f / ((float)options.size());
			Math::Vec2f textPos = { 0.f, -yInc };

			for (auto& option : options)
			{
				if (mSelectedOption == option)
				{
					std::string opText(GetText(option));
					opText = "  " + opText + "  ";
					
					// get current frame from animated sprite and attach it to text
					opText[0] = selectHighlightLeft.frames[selectHighlightLeft.currentFrame].data[0][0];
					opText[opText.length() - 1] = selectHighlightRight.frames[selectHighlightRight.currentFrame].data[0][0];

					pView->DrawTextCentered(opText, textPos);
				}
				else
					pView->DrawTextCentered(GetText(option), textPos);

				textPos.y += yInc;
			}
		}
	}

	std::function<void(GameManager*)> MainMenu::GetEventElevatedFn()
	{
		return [](GameManager* gm)
		{
			gm->level = std::make_unique<DefaultMap>();
		};
	}

	std::string_view MainMenu::GetText(Option option)
	{
		switch (option)
		{
		case Option::StartGame:
			return "Start Game";
		case Option::LevelSelect:
			return "Level Select";
		case Option::Quit:
			return "Quit";
		}

		return "";
	}
	
	DefaultMap::DefaultMap()
		: levelExtent(Core::Application::Get().GetTerminal().GetScreenExtent())
	{
		auto& head = mSnake.Head();
		head = { levelExtent.x / 2, levelExtent.y / 2 };

		RelocateFruit();
	}
	
	DefaultMap::~DefaultMap()
	{
	}
	
	void DefaultMap::OnUpdate(float delta)
	{
		if (!Utils::InBound(mFruitPos, levelExtent))
			RelocateFruit();

		float speed = levelExtent.y / mFullDuration;

		static float acc = 0.f;
		acc += delta;
		if (acc * speed > 1.f)
		{
			acc = 0.f;
			mOldDirection = mNewDirection;
			if (mSnake.Move(mNewDirection))
			{
				mGameOver = true;
				return;
			}

			auto& snakeHead = mSnake.Head();

			// bounds checking
			if (snakeHead.x < 0)
				snakeHead.x = levelExtent.x - 1;
			else if (snakeHead.x >= levelExtent.x)
				snakeHead.x = 0;
			else if (snakeHead.y < 0)
				snakeHead.y = levelExtent.y - 1;
			else if (snakeHead.y >= levelExtent.y)
				snakeHead.y = 0;

			// fruit checking
			if (snakeHead == mFruitPos)
			{
				mSnake.Grow();
				RelocateFruit();
			}
		}

	}

	bool DefaultMap::OnEvent(Core::Event e)
	{
		static const Math::Vec2i directions[4] =
		{
			{  0,  1 }, // 0 -> down
			{  0, -1 }, // 1 -> up
			{  1,  0 }, // 2 -> right
			{ -1,  0 }  // 3 -> left
		};

		// return to main menu after pressing any key
		if (mGameOver)
			return true;

		switch (e)
		{
		case Core::Event::Down:
			if (mOldDirection != directions[1])
				mNewDirection = { 0, 1 };
			break;
		case Core::Event::Up:
			if (mOldDirection != directions[0])
				mNewDirection = { 0, -1 };
			break;
		case Core::Event::Right:
			if (mOldDirection != directions[3])
				mNewDirection = { 1, 0 };
			break;
		case Core::Event::Left:
			if (mOldDirection != directions[2])
				mNewDirection = { -1, 0 };
			break;
		case Core::Event::Escape:
			return true;
		}

		return false;
	}
	
	void DefaultMap::FillView(Core::View* pView)
	{
		if (mGameOver)
		{
			pView->DrawTextCentered("Game Over!", Math::Vec2f(0.f, -0.1f));

			auto score = "Score: " + std::to_string(mSnake.body.size());
			pView->DrawTextCentered(score, Math::Vec2f(0.f, 0.f));

			pView->DrawTextCentered("Press any key to return to Main Menu", Math::Vec2f(0.f, 0.2f));

			return;
		}

		levelExtent = pView->GetExtent();

		// body
		for (auto it = mSnake.body.begin() + 1; it != mSnake.body.end(); it++)
			pView->DrawSprite({ {"#"} }, *it);

		// fruit
		pView->DrawSprite({ {"F"} }, mFruitPos);

		// head
		Core::Sprite head = { {" "} };

		// change head sprite based on direction
		if (mOldDirection.x == 1)
			head.data[0][0] = '>';
		else if (mOldDirection.x == -1)
			head.data[0][0] = '<';
		else if (mOldDirection.y == 1)
			head.data[0][0] = 'v';
		else if (mOldDirection.y == -1)
			head.data[0][0] = '^';

		pView->DrawSprite(head, mSnake.Head());
	}

	std::function<void(GameManager*)> DefaultMap::GetEventElevatedFn()
	{
		return [](GameManager* gm)
		{
			gm->level = std::make_unique<MainMenu>();
		};
	}

	void DefaultMap::RelocateFruit()
	{
		while (true)
		{
			bool conflict = false;
			
			mFruitPos = Math::RandVec2<int32_t>(0, levelExtent.x - 1, 0, levelExtent.y - 1);

			for (auto& pos : mSnake.body)
				if (pos == mFruitPos)
				{
					conflict = true;
					break;
				}
			
			if (!conflict)
				break;
		}
	}
}