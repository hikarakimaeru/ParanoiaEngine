	#pragma once

	#include "../Scene/gameObject.h"
	#include <algorithm>

	class Colider {

	public:
		static bool checkCollision(const gameObject& a, const gameObject& b) {
			glm::vec2 a_pos = a.getPosition();
			glm::vec2 a_size = a.transform.size;

			glm::vec2 b_pos = b.getPosition();
			glm::vec2 b_size = b.transform.size;

			bool collision_X = (a_pos.x + a_size.x >= b_pos.x) && (a_pos.x <= b_pos.x + b_size.x);
			bool collision_Y = (a_pos.y + a_size.y >= b_pos.y) && (a_pos.y <= b_pos.y + b_size.y);

			return collision_X && collision_Y;
		}

		static bool resolveCollision(gameObject& a, gameObject& b) {
			glm::vec2 a_pos = a.getPosition();
			glm::vec2 a_size = a.transform.size;

			glm::vec2 b_pos = b.getPosition();
			glm::vec2 b_size = b.transform.size;

			float overlapLeft = (a_pos.x + a_size.x) - b_pos.x;
			float overlapRight = (b_pos.x + b_size.x) - a_pos.x;
			float overlapTop = (a_pos.y + a_size.y) - b_pos.y;
			float overlapBottom = (b_pos.y + b_size.y) - a_pos.y;

			float minOverlapX = std::min(overlapLeft, overlapRight);
			float minOverlapY = std::min(overlapTop, overlapBottom);

			if (minOverlapX < minOverlapY) {
				if (overlapLeft < overlapRight) {
					a.setPosition(a_pos.x - overlapLeft, a_pos.y);
				}
				else {
					a.setPosition(a_pos.x + overlapRight, a_pos.y);
				}
			}
			else {
				if (overlapTop < overlapBottom) {
					a.setPosition(a_pos.x, a_pos.y - overlapTop);
				}
				else {
					a.setPosition(a_pos.x, a_pos.y + overlapBottom);
				}
			}
		}
	};