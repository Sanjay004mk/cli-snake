#include "game.h"
#include "../core/view.h"
#include "../core/screen.h"

#include <iostream>
#include <chrono>

namespace Game
{
	// temp
	static Core::Sprite player = {
		{
			{'-', '*', '0'},
			{'-', '-', '/'}
		}
	};

	GameManager::GameManager()
	{
		Core::Input::Init();
		Core::Input::SetEventCallbackFn([this](Core::Event e) { this->OnEvent(e); });	
	}

	GameManager::~GameManager()
	{
		Core::Input::Shutdown();
	}
	
	void GameManager::Run()
	{
		std::chrono::steady_clock::time_point last, now = std::chrono::high_resolution_clock::now();
		while (m_Running)
		{
			last = now;

			m_Screen.OnUpdate(m_Delta);

			Core::View& view = m_Screen.PrepareView();

			// tmp
			{
				static Math::Vec2f position = { -1.f, -1.f };
				static Math::Vec2f direction = { 1.f, 0.f };

				static const Math::Vec2f dirUp(0.f, -1.f);
				static const Math::Vec2f dirDown(0.f, 1.f);
				static const Math::Vec2f dirLeft(-1.f, 0.f);
				static const Math::Vec2f dirRight(1.f, 0.f);

				view.Draw(player, position);

				auto e = view.GetCharExtent();
				position += direction * e * m_Delta * 5.f;
				if (position.x >= 1.f && direction == dirRight)
					direction = dirDown;
				if (position.y >= 1.f && direction == dirDown)
					direction = dirLeft;
				if (position.x <= -1.f && direction == dirLeft)
					direction = dirUp;
				if (position.y <= -1.f && direction == dirUp)
					direction = dirRight;

			}

			m_Screen.FlushView();

			Core::Input::PollEvents(m_Delta);

			now = std::chrono::high_resolution_clock::now();
			m_Delta = std::min(0.33f, std::chrono::duration_cast<std::chrono::microseconds>(now - last).count() * 1e-6f);
		}
	}
	
	void GameManager::OnEvent(Core::Event e)
	{
		if (e == Core::Event::Escape)
			m_Running = false;
	}
}