#pragma once
#include <functional>

namespace Core
{
	enum class Event
	{
		Up,
		Down,
		Left,
		Right,
		Enter,
		Escape,
		Invalid,
	};

	class Input
	{
	public:
		using EventCallbackFn = std::function<void(Event)>;

		static void Init();
		static void Shutdown();
		static void SetEventCallbackFn(EventCallbackFn fn);
		static void PollEvents();

	private:
		static inline EventCallbackFn s_CallbackFn;
		static inline bool s_InEvent[(int)Event::Invalid];
	};
}