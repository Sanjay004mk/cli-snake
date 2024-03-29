#include "../core/entrypoint.h"
#include <iostream>
#include "game.h"

namespace Core
{
	Application* CreateApplication(const CommandLineArgs& args)
	{
		auto app = new Application();
		app->AddAttachment(std::make_shared<Game::GameManager>());
		return app;
	}
}

namespace Game
{	
	GameManager::GameManager()
	{
		level = std::make_unique<MainMenu>();
	}

	GameManager::~GameManager()
	{
	}

	void GameManager::OnUpdate(Core::View& view, float delta)
	{
		// tmp
		/*{
			static Math::Vec2f position = { -1.f, -1.f };
			static Math::Vec2f direction = { 1.f, 0.f };

			static const Math::Vec2f dirUp(0.f, -1.f);
			static const Math::Vec2f dirDown(0.f, 1.f);
			static const Math::Vec2f dirLeft(-1.f, 0.f);
			static const Math::Vec2f dirRight(1.f, 0.f);

			view.DrawSprite(player, position);
			view.DrawAnimatedSprite(loading, Math::Vec2f(0.f, 0.f));

			auto e = view.GetCharExtent();
			position += direction * e * delta * 5.f;
			if (position.x >= 1.f && direction == dirRight)
				direction = dirDown;
			if (position.y >= 1.f && direction == dirDown)
				direction = dirLeft;
			if (position.x <= -1.f && direction == dirLeft)
				direction = dirUp;
			if (position.y <= -1.f && direction == dirUp)
				direction = dirRight;

		}*/

		level->OnUpdate(delta);
		level->FillView(&view);
	}

	bool GameManager::OnEvent(Core::Event e)
	{
		/*if (e == Core::Event::Escape)
		{
			Core::Application::Get().Close();
			return true;
		}*/
		
		if (level->OnEvent(e))
		{
			auto fn = level->GetEventElevatedFn();
			fn(this);
		}

		return false;
	}
}
