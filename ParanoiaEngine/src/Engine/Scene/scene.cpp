#include "Scene.h"
#include "../Graphics/batchRenderer.h"
#include <algorithm>

Scene::~Scene() {
    clear();
}

void Scene::addObject(gameObject* obj) {
    if (obj) {
        objects.push_back(obj);
    }
}

void Scene::update(float dt) {
    for (gameObject* obj : objects) {
        if (obj) {
            obj->update(dt);
        }
    }
}

void Scene::draw(batchRenderer& renderer, const Camera& camera) {
    Texture* currentTexture = nullptr;

    renderer.beginBatch(camera);

    for (gameObject* obj : objects) {
        if (!obj) continue;

        if (obj->texture && obj->texture != currentTexture) {
            renderer.flush();

            currentTexture = obj->texture;
            currentTexture->bind(0);

            renderer.shader_program->use();
            renderer.shader_program->setInt("uTexture", 0);

            renderer.beginBatch(camera);
        }

        obj->draw(renderer);
    }

    renderer.flush();
}

void Scene::clear() {
    for (gameObject* obj : objects) {
        delete obj;
    }
    objects.clear();
}

void Scene::removeObject(gameObject* obj) {
    for (size_t i = 0; i < objects.size(); i++) {
        if (objects[i] == obj) {
            delete objects[i];
            objects.erase(objects.begin() + i);
            return;
        }
    }
}