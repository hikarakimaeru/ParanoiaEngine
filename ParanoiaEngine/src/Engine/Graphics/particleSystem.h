#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "batchRenderer.h"
#include "camera.h"
#include "texture.h"

struct Particle {
	glm::vec2 pos;
	glm::vec2 vel;
	float lifeTime;
	float maxLifeTime;
	float size;
	glm::vec3 color;
};

class ParticleSystem {
public:
	void draw(batchRenderer& renderer, const Camera& camera, Texture& tex);
	void emit(glm::vec2 pos, int count, glm::vec3 color);
	float randomRange(float num1, float num2);
	void update(float dt);
private:
	std::vector<Particle> particles;
};