#include "game.h"
#include "../core/view.h"
#include "../core/screen.h"

#include <iostream>

namespace Game
{
	GameManager::GameManager()
	{
		Core::Input::Init();
		Core::Input::SetEventCallbackFn([this](Core::Event e) { this->OnEvent(e); });

		// temp
		Core::Screen& screen = m_Screen;
		std::cout << screen.GetScreenExtent() << std::endl;
		std::cout << screen.GetAspectRatio() << std::endl;
		std::cout << screen.WorldSpaceToScreenSpace({ 1.f, 1.f }) << std::endl;

		Core::Sprite player = {
			{
				{'-', '*', '0'},
				{'-', '-', '/'}
			}
		};
		
		Core::View& view = screen.PrepareView();
		view.Draw(player, Math::Vec2f(1.f, (1.f - 3.f * (1.f / 15.f))));

		screen.FlushView();
	}

	GameManager::~GameManager()
	{
		Core::Input::Shutdown();
	}
	
	void GameManager::Run()
	{
	}
	
	void GameManager::OnEvent(Core::Event e)
	{
	}
}