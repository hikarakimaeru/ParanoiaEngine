#pragma once

#include <glm/glm.hpp>

class Transform {
public:
    glm::vec2 position{ 0.0f, 0.0f };
    glm::vec2 size{ 1.0f, 1.0f };
    float rotation{ 0.0f };

    Transform() = default;
    Transform(const glm::vec2& pos, const glm::vec2& sz, float rot = 0.0f);

    glm::mat4 getMatrix() const;
};