#include "input.h"

#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>

#elif defined(__linux__)

#include <sys/ioctl.h>

#endif // Windows/Linux

namespace Core
{
	void Input::Init()
	{
	}
	
	void Input::Shutdown()
	{
	}
	
	void Input::SetEventCallbackFn(EventCallbackFn fn)
	{
		s_CallbackFn = fn;
	}
	
	void Input::PollEvents(float delta)
	{
		static float acc = 0.f;
		acc += delta;

#if defined(_WIN32)

		if (GetAsyncKeyState(VK_BACK))
			s_CallbackFn(Event::Escape);

#elif defined(__linux__)

		if (acc >= 5.f)
			s_CallbackFn(Event::Escape);

#endif // Windows/Linux

	}
}