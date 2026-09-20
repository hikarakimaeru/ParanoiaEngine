#include "transform.h"
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform(const glm::vec2& pos, const glm::vec2& sz, float rot)
    : position(pos), size(sz), rotation(rot) {
}

glm::mat4 Transform::getMatrix() const {
    glm::mat4 mat = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));

    if (rotation != 0.0f) {
        mat = glm::rotate(mat, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    mat = glm::scale(mat, glm::vec3(size, 1.0f));
    return mat;
}