#pragma once

#include <vector>

#include "../math/vec.h"

namespace Game
{
	struct Snake
	{
		std::vector<Math::Vec2i> body = { {} };

		Math::Vec2i& Head() { return body[0]; }

		bool Move(const Math::Vec2i& direction)
		{
			for (auto it = body.rbegin(); it != body.rend() - 1; it++)
				*it = *(it + 1);

			body[0] += direction;
			
			// detect collision
			for (auto it = body.begin() + 1; it != body.end(); it++)
				if (*it == Head())
					return true;

			return false;
		}

		void Grow()
		{
			body.push_back(body.back());
		}
	};
}