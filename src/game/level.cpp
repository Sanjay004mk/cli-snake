#include <functional>

#include "level.h"

namespace Utils
{
	static bool CheckSize(const Core::View& view, const Math::Vec2i& minSize)
	{
		return (view.GetWidth() < minSize.x || view.GetHeight() < minSize.y);
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

	void MainMenu::OnEvent(Core::Event e)
	{
		switch (e)
		{
		case Core::Event::Down:
			mSelectedOption = (Option)(((int)mSelectedOption + 1) % (int)Option::Invalid);
			break;
		case Core::Event::Up:
			mSelectedOption = (Option)(((int)mSelectedOption - 1) < 0 ? ((int)Option::Invalid - 1) : ((int)mSelectedOption - 1));
			break;
		}
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
	{
	}
	
	DefaultMap::~DefaultMap()
	{
	}
	
	void DefaultMap::OnEvent(Core::Event e)
	{
	}
	
	void DefaultMap::FillView(Core::View* pView)
	{
	}
}