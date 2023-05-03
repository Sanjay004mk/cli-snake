#pragma once
#include "../core/input.h"
#include "../core/screen.h"

namespace Game
{
	class GameManager
	{
	public:
		GameManager();
		~GameManager();

		void Run();

	private:
		void OnEvent(Core::Event e);

		bool s_Running = true;
		Core::Screen m_Screen;
	};
}