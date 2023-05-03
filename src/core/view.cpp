#include "view.h"

namespace Core
{
	View::View(const Math::Vec2i& viewExtent) 
		: m_ViewExtent(viewExtent)
	{
		m_View.resize(viewExtent.x * viewExtent.y, 0);
	}
	void View::Draw(const Sprite& sprite, const Math::Vec2i& position)
	{
		int32_t px = position.x, py = position.y, sx = sprite.GetWidth(), sy = sprite.GetHeight();

		// sprite out of view
		if (px >= m_ViewExtent.x || py >= m_ViewExtent.y)
			return;

		// cull sprite extent to be inside view
		int32_t pxmax = std::min(px + sx, m_ViewExtent.x);
		int32_t pymax = std::min(py + sy, m_ViewExtent.y);

		for (int32_t x = px; x < pxmax; x++)
			for (int32_t y = py; y < pymax; y++)
				// this[y][x] because of how we are indexing the array
				(*this)[y][x] = sprite[x - px][y - py];
	}

}