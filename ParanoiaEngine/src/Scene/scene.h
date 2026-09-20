#pragma once

#include <vector>
#include <string>
#include "gameObject.h"

class batchRenderer;
class Camera;

class Scene {
private:
    std::vector<gameObject*> objects;

public:
    Scene() = default;
    ~Scene();

    void addObject(gameObject* obj);

    void update(float dt);

    void draw(batchRenderer& renderer, const Camera& camera);

    void clear();
};