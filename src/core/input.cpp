#include "input.h"

#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>

#elif defined(__linux__)

#include <ncurses.h>

#endif // Windows/Linux

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
		
#if defined(_WIN32)

		if (GetAsyncKeyState(VK_BACK))
			s_CallbackFn(Event::Escape);

#elif defined(__linux__)

		auto ch = getch();
		if (ch != ERR)
		{
			if (ch == KEY_BACKSPACE)
				s_CallbackFn(Event::Escape);
			else if (ch == KEY_UP || ch == 'w')
				s_CallbackFn(Event::Up);
			else if (ch == KEY_DOWN || ch == 's')
				s_CallbackFn(Event::Down);
			else if (ch == KEY_LEFT || ch == 'a')
				s_CallbackFn(Event::Left);
			else if (ch == KEY_RIGHT || ch == 'd')
				s_CallbackFn(Event::Right);
			else if (ch == KEY_ENTER)
				s_CallbackFn(Event::Enter);
		}

#endif // Windows/Linux

	}
}