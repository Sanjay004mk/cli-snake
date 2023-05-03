#pragma once
#include "../core/input.h"

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

	};
}