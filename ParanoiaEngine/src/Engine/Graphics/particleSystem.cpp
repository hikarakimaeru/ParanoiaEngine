#include "particleSystem.h"
#include <random>
#include <cmath>

float ParticleSystem::randomRange(float num1, float num2) {
    static std::mt19937 gen{ std::random_device{}() };
    std::uniform_real_distribution<float> d(num1, num2);
    return d(gen);
}

void ParticleSystem::emit(glm::vec2 pos, int count, glm::vec3 color) {
    for (int i = 0; i < count; i++) {
        float angle = randomRange(0.0f, 6.2832f);
        float speed = randomRange(50.0f, 200.0f);
        Particle p;
        p.pos = pos;
        p.vel = { std::cos(angle) * speed, std::sin(angle) * speed };
        p.maxLifeTime = randomRange(0.3f, 0.7f);
        p.lifeTime = p.maxLifeTime;
        p.size = randomRange(4.0f, 8.0f);
        p.color = color;
        particles.push_back(p);
    }
}

void ParticleSystem::update(float dt) {
    for (size_t i = 0; i < particles.size();) {
        particles[i].lifeTime -= dt;
        if (particles[i].lifeTime <= 0.0f) {
            particles.erase(particles.begin() + i);
        }
        else {
            particles[i].pos += particles[i].vel * dt;
            i++;
        }
    }
}

void ParticleSystem::draw(batchRenderer& renderer, const Camera& camera, Texture& tex) {
    if (particles.empty()) return;

    renderer.beginBatch(camera);
    tex.bind(0);
    for (const Particle& p : particles) {
        float alpha = p.lifeTime / p.maxLifeTime;
        renderer.drawQuad(p.pos, { p.size, p.size }, { p.color.r, p.color.g, p.color.b, alpha }, { 0.0f, 0.0f }, { 1.0f, 1.0f });
    }
    renderer.flush();
}