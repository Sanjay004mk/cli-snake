#pragma once
#include "input.h"
#include "view.h"

namespace Core
{
	class Attachment
	{
	public:
		virtual void OnUpdate(View& view, float delta) = 0;
		virtual bool OnEvent(Event e) = 0;
	};
}