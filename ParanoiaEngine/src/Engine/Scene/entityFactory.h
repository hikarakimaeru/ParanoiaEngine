#pragma once

#include "gameObject.h"
#include "scene.h"
#include "../Graphics/texture.h"

class EntityFactory {
public:
	static gameObject* createEnemy(Scene& scene, const std::string& name, Texture* texture, glm::vec2 position) {
		gameObject* enemy = new gameObject(name, texture);

		enemy->setPosition(position);
		enemy->transform.size = { 64.0f, 64.0f };

		scene.addObject(enemy);

		return enemy;
	}
};