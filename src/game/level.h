#pragma once
#include "../core/input.h"
#include "../math/vec.h"
#include "../core/view.h"

#include <vector>
#include <string>
#include <functional>

#include "snake.h"

namespace Game
{
	class GameManager;

	class Level
	{
	public:
		Level() {}
		virtual ~Level() {}

		virtual void OnUpdate(float delta) {}
		virtual bool OnEvent(Core::Event e) { return false; }
		virtual void FillView(Core::View* pView) {}
		virtual std::function<void(GameManager*)> GetEventElevatedFn() { return [](GameManager* gm) {}; }

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

		virtual bool OnEvent(Core::Event e) override;
		virtual void FillView(Core::View* pView) override;
		virtual std::function<void(GameManager*)> GetEventElevatedFn() override;

	private:
		std::string_view GetText(Option option);

		Option mSelectedOption = Option::StartGame;
		
	};

	class DefaultMap : public Level
	{
	public:
		DefaultMap();
		~DefaultMap();

		virtual void OnUpdate(float delta) override;
		virtual bool OnEvent(Core::Event e) override;
		virtual void FillView(Core::View* pView) override;
		virtual std::function<void(GameManager*)> GetEventElevatedFn() override;

		Math::Vec2i levelExtent = {};
	private:
		void RelocateFruit();

		bool mGameOver = false;

		Snake mSnake;
		float mFullDuration = 5.f;
		Math::Vec2i mNewDirection = { 0, -1 };
		Math::Vec2i mOldDirection = { 0, -1 };
		Math::Vec2i mFruitPos = {};
	};
}