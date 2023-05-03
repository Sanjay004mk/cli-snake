#pragma once
#include "view.h"

namespace Core
{
	class Screen
	{
	public:
		Screen();
		~Screen();

		void OnUpdate(float ts);

		void Draw(const View& view);

		Math::Vec2i GetScreenExtent() const { return m_ScreenExtent; }
		float GetAspectRatio() const;

		Math::Vec2i WorldSpaceToScreenSpace(const Math::Vec2f& vector) const;
		Math::Vec2f ScreenSpaceToWorldSpace(const Math::Vec2i& vector) const;

	private:
		Math::Vec2i m_ScreenExtent = {};
	};
}