#include "level.h"

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
			mOption = (Option)(((int)mOption + 1) % (int)Option::Invalid);
			break;
		case Core::Event::Up:
			mOption = (Option)(((int)mOption - 1) < 0 ? ((int)Option::Invalid - 1) : ((int)mOption - 1));
			break;
		}
	}

	void MainMenu::FillView(Core::View* pView)
	{
		pView->DrawTextCentered(GetText(mOption), Math::Vec2f(0.f, 0.f));
	}

	std::string MainMenu::GetText(Option option)
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
		return std::string();
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