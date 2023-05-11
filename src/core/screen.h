#pragma once
#include "view.h"

#include <memory>

namespace Core
{
	class Screen
	{
	public:
		Screen();
		~Screen();

		void OnUpdate(float ts);
		void FlushView();

		Math::Vec2i GetScreenExtent() const { return m_ScreenExtent; }

		float GetAspectRatio() const;

		float GetCharWidth() const { return m_View->GetCharWidth(); }
		float GetCharHeight() const { return m_View->GetCharHeight(); }
		Math::Vec2f GetCharExtent() const { return m_View->GetCharExtent(); }

		Math::Vec2i WorldSpaceToScreenSpace(const Math::Vec2f& vector) const;
		Math::Vec2f ScreenSpaceToWorldSpace(const Math::Vec2i& vector) const;

		View& GetView() { return *m_View; }
		View& PrepareView(); 

		void ClearScreen();

	private:
		void OnResize();
		bool UpdateViewSize();

		Math::Vec2i m_ScreenExtent = {};
		std::shared_ptr<View> m_View;
		std::shared_ptr<View> m_OldView;
	};
}