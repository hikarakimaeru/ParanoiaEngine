#pragma once

#include <string>
#include <glm/glm.hpp>

#include "../Core/transform.h"
#include "../Graphics/texture.h"

class batchRenderer;

class gameObject {
public:
    std::string name;
    Texture* texture = nullptr;
    Transform transform;

    gameObject(const std::string& name = "GameObject", Texture* texture = nullptr);
    virtual ~gameObject() = default;

    virtual void update(float dt);

    void draw(batchRenderer& renderer);

    glm::vec2 getPosition() const;
    void setPosition(const glm::vec2& pos);
    void setPosition(float x, float y);
    void move(float dx, float dy);
    void move(const glm::vec2& offset);
};