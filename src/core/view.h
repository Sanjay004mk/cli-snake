#pragma once
#include "../math/vec.h"

#include <vector>

namespace Core
{
	struct Sprite
	{
		// size must always be greater than (0, 0)
		Sprite(const Math::Vec2i& size) 
		{
			data.resize(size.x);
			for (auto& d : data)
				d.resize(size.y);
		}
		Sprite(const std::vector<std::vector<char>>& data) : data(data) {}

		std::vector<char>& operator[](int32_t i) { return data[i]; }
		const std::vector<char>& operator[](int32_t i) const { return data[i]; }

		int32_t GetWidth() const { return (int32_t)data[0].size(); }
		int32_t GetHeight() const { return (int32_t)data.size(); }
		Math::Vec2i GetExtent() const { return { GetWidth(), GetHeight() }; }

		std::vector<std::vector<char>> data;
	};

	class View
	{
	public:
		View(const Math::Vec2i& viewExtent);

		void Draw(const Sprite& sprite, const Math::Vec2i& top_left_position);

		const Math::Vec2i& GetExtent() const { return m_ViewExtent; }
		const int32_t GetWidth() const { return m_ViewExtent.x; }
		const int32_t GetHeight() const { return m_ViewExtent.y; }

		char* operator[](int32_t i) { return &m_View[i * m_ViewExtent.x]; }

	private:
		Math::Vec2i m_ViewExtent = {};
		std::vector<char> m_View = {};
	};
}