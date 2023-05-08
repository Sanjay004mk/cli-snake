#include "view.h"

namespace Utils
{
	static float LerpN1to1(float t, float v1, float v2)
	{
		t = (t + 1.f) / 2.f;
		return v2 * t + (1.f - t) * v1;
	}
}

namespace Core
{
	View::View(const Math::Vec2i& viewExtent) 
		: m_ViewExtent(viewExtent)
	{
		m_View.resize(viewExtent.x);
		for (auto& v : m_View)
			v.resize(viewExtent.y);
	}
	
	void View::Clear(char clearChar)
	{
		for (auto& v : m_View)
			for (auto& c : v)
				c = clearChar;
	}

	void View::Draw(const Sprite& sprite, const Math::Vec2i& position)
	{
		int32_t px = position.x, py = position.y, sx = sprite.GetWidth(), sy = sprite.GetHeight();

		// sprite out of view
		if (px >= m_ViewExtent.x || py >= m_ViewExtent.y)
			return;

		// cull sprite extent to be inside view
		int32_t pxmax = std::min(px + sy, m_ViewExtent.x);
		int32_t pymax = std::min(py + sx, m_ViewExtent.y);

		for (int32_t x = px; x < pxmax; x++)
			for (int32_t y = py; y < pymax; y++)
				(*this)[x][y] = sprite[y - py][x - px];
	}

	void View::Draw(const Sprite& sprite, const Math::Vec2f& world_position)
	{
		Draw(sprite, WorldSpaceToScreenSpace(world_position));
	}

	float View::GetAspectRatio() const
	{
		return (float)m_ViewExtent.x / (float)m_ViewExtent.y;
	}

	Math::Vec2i View::WorldSpaceToScreenSpace(const Math::Vec2f& v) const
	{
		// convert to (-1, -1) -> (1, 1) 
		auto vector = v;
		float aspectRatio = GetAspectRatio();
		if (aspectRatio > 1.f)
			vector.x /= aspectRatio;
		else
			vector.y *= aspectRatio;

		return Math::Vec2i((int32_t)Utils::LerpN1to1(vector.x, 0.f, (float)m_ViewExtent.x), (int32_t)Utils::LerpN1to1(vector.y, 0.f, (float)m_ViewExtent.y));
	}

	Math::Vec2f View::ScreenSpaceToWorldSpace(const Math::Vec2i& vector) const
	{
		float x = ((float)vector.x / (float)m_ViewExtent.x) * 2.f - 1.f, y = ((float)vector.y / (float)m_ViewExtent.y) * 2.f - 1.f;
		return Math::Vec2f(x, y);
	}

}