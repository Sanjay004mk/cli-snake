#pragma once
#include <functional>

namespace Core
{
	enum class Event
	{
		Invalid = 0,
		Up,
		Down,
		Left,
		Right,
		Enter,
		Escape,
	};

	class Input
	{
	public:
		using EventCallbackFn = std::function<void(Event)>;

		static void Init();
		static void Shutdown();
		static void SetEventCallbackFn(EventCallbackFn fn);
		static void PollEvents(float delta);

	private:
		static inline EventCallbackFn s_CallbackFn;
	};
}