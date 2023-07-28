#include "input.h"

#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>

#elif defined(__linux__)

#include <ncurses.h>

#endif // Windows/Linux

namespace Utils
{
#if defined(_WIN32)
	
	int GetVKC(Core::Event e)
	{
		switch (e)
		{
		case Core::Event::Escape:
			return VK_BACK;
		case Core::Event::Enter:
			return VK_RETURN;
		case Core::Event::Down:
			return VK_DOWN;
		case Core::Event::Left:
			return VK_LEFT;
		case Core::Event::Right:
			return VK_RIGHT;
		case Core::Event::Up:
			return VK_UP;
		}
	}

#elif defined(__linux__)

	int GetVKC(Core::Event e)
	{
		switch (e)
		{
		case Core::Event::Escape:
			return KEY_BACKSPACE;
		case Core::Event::Enter:
			return KEY_ENTER;
		case Core::Event::Down:
			return KEY_DOWN;
		case Core::Event::Left:
			return KEY_LEFT;
		case Core::Event::Right:
			return KEY_RIGHT;
		case Core::Event::Up:
			return KEY_UP;
}
	}

#endif // Windows/Linux
}

namespace Core
{
	void Input::Init()
	{
#if defined(__linux__)
		nodelay(stdscr, true);
		keypad(stdscr, true);
#endif
	}
	
	void Input::Shutdown()
	{
	}
	
	void Input::SetEventCallbackFn(EventCallbackFn fn)
	{
		s_CallbackFn = fn;
	}
	
	void Input::PollEvents()
	{
		for (int i = 0; i < (int)Event::Invalid; i++)
		{
			int vkc = Utils::GetVKC((Event)i);
			if (GetAsyncKeyState(vkc))
			{
				if (!s_InEvent[i])
				{
					s_CallbackFn((Event)i);
					s_InEvent[i] = true;
				}
			}
			else
				s_InEvent[i] = false;
		}
	}
}