#pragma once
#include "../core/input.h"
#include "../math/vec.h"
#include "../core/view.h"

#include <vector>
#include <string>

namespace Game
{
	class Level
	{
	public:
		Level() {}
		virtual ~Level() {}

		virtual void OnEvent(Core::Event e) = 0;
		virtual void FillView(Core::View* pView) = 0;

	private:
	};

	class MainMenu : public Level
	{
		enum class Option
		{
			StartGame = 0,
			LevelSelect,
			Quit,

			Invalid
		};

	public:
		MainMenu();
		~MainMenu();

		virtual void OnEvent(Core::Event e) override;
		virtual void FillView(Core::View* pView) override;

	private:
		std::string_view GetText(Option option);

		Option mSelectedOption = Option::StartGame;
		
	};

	class DefaultMap : public Level
	{
	public:
		DefaultMap();
		~DefaultMap();

		virtual void OnEvent(Core::Event e) override;
		virtual void FillView(Core::View* pView) override;

	private:
	};
}