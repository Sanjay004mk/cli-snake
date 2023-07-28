#pragma once
#include "../math/vec.h"

#include <vector>
#include <string_view>

namespace Core
{
	struct Sprite
	{
		// construct Sprite with initializer list of strings
		Sprite(const std::vector<const char*>& data);

		std::vector<char>& operator[](int32_t i) { return data[i]; }
		const std::vector<char>& operator[](int32_t i) const { return data[i]; }

		int32_t GetWidth() const { return (int32_t)data.size(); }
		int32_t GetHeight() const { return (int32_t)data[0].size(); }
		Math::Vec2i GetExtent() const { return { GetWidth(), GetHeight() }; }

		std::vector<std::vector<char>> data;
	};

	struct AnimatedSprite
	{
		uint32_t fps;
		uint32_t currentFrame = 0;
		float time = 0.f;
		std::vector<Sprite> frames;

		void Reset() { currentFrame = 0; time = 0.f; }
	};

	class View
	{
	public:
		View(const Math::Vec2i& viewExtent);

		void Clear(char clearChar = ' ');

		void DrawSprite(const Sprite& sprite, const Math::Vec2i& top_left_position);
		void DrawSprite(const Sprite& sprite, const Math::Vec2f& world_position);

		// returns true when animation is complete
		bool DrawAnimatedSprite(AnimatedSprite& sprite, const Math::Vec2f& position);
		bool DrawAnimatedSprite(AnimatedSprite& sprite, const Math::Vec2i& position);

		// broken
		void DrawLine(const Math::Vec2f& p1, const Math::Vec2f& p2, const Sprite& style = { {"*"} });
		
		void DrawText(std::string_view text, const Math::Vec2f& position);
		void DrawText(std::string_view text, const Math::Vec2i& position);
		void DrawTextCentered(std::string_view text, const Math::Vec2f& position);
		void DrawTextCentered(std::string_view text, const Math::Vec2i& position);

		const Math::Vec2i& GetExtent() const { return m_ViewExtent; }
		const int32_t GetWidth() const { return m_ViewExtent.x; }
		const int32_t GetHeight() const { return m_ViewExtent.y; }

		float GetAspectRatio() const;

		// returns the size of one character in x axis
		float GetCharWidth() const;
		// returns the size of one character in y axis
		float GetCharHeight() const;
		// returns the size of one character 
		Math::Vec2f GetCharExtent() const { return { GetCharWidth(), GetCharHeight() }; }

		Math::Vec2i WorldSpaceToScreenSpace(const Math::Vec2f& vector) const;
		Math::Vec2f ScreenSpaceToWorldSpace(const Math::Vec2i& vector) const;

		std::vector<char>& operator[](int32_t i) { return m_View[i]; }
		const std::vector<char>& operator[](int32_t i) const { return m_View[i]; }

	private:
		Math::Vec2i m_ViewExtent = {};
		std::vector<std::vector<char>> m_View = {};
	};
}