#pragma once
#include <vector>

#include "../math/vec.h"

using Vec2 = Math::Vec2f;


#define SNAKE_HEAD '@'
#define SNAKE_BODY '#'
#define FRUIT	   'O'
#define WIDTH       20
#define HEIGHT	    20

namespace Game
{

	struct Snake
	{
		std::vector<Vec2> parts;
		Vec2 Direction;

		Snake(const Vec2& position = Vec2(9.f, 9.f), const Vec2& direction = Vec2(1.f, 0.f))
			: Direction(direction)
		{
			// initial size = 2 (1 head, 1 tail)
			parts.push_back(position + direction);
			parts.push_back(position);
		}

		const Vec2& Head() const { return parts[0]; }

		void Grow()
		{
			// set new part to be same as tail
			// it will be changed when the snake moves
			parts.push_back(parts.back());
		}

		bool ChangeDir(const Vec2& dir)
		{
			// if player tries to reverse direction
			// ignore it
			if (parts[0] + dir == parts[1])
				return false;

			Direction = dir;
			return true;
		}

		void Move(float x_max = WIDTH, float y_max = HEIGHT)
		{
			// shift all parts 1 step back (1 = 0, 2 = 1, 3 = 2, .... )
			std::memmove(&parts[1], &parts[0], (parts.size() - 1) * sizeof(Vec2));
			// move the first part (head) by direction
			auto& part = (parts[0] += Direction);

			// loop back around if head goes out of map
			if (part.x >= WIDTH)
				part.x = 0.f;
			else if (part.x < 0.f)
				part.x = WIDTH - 1.f;

			if (part.y >= HEIGHT)
				part.y = 0.f;
			else if (part.y < 0.f)
				part.y = HEIGHT - 1.f;
		}
	};
}