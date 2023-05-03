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

		Core::Screen screen;
		std::cout << screen.GetScreenExtent() << std::endl;
		std::cout << screen.GetAspectRatio() << std::endl;
		std::cout << screen.WorldSpaceToScreenSpace({ 1.f, 1.f }) << std::endl;

		// temp
		Core::Sprite player = {
			{
				{'-', '*'},
				{'-', '-'}
			}
		};
		Core::View view({ 20, 20 });
		view.Draw(player, { 0, 0 });

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