#include "gameObject.h"
#include "../Graphics/batchRenderer.h"

gameObject::gameObject(const std::string& name, Texture* texture)
    : name(name), texture(texture) {
}

void gameObject::update(float dt) {
    // Логика обновления
}

void gameObject::draw(batchRenderer& renderer) {
    renderer.drawQuad(transform.position, transform.size);
}

glm::vec2 gameObject::getPosition() const {
    return transform.position;
}

void gameObject::setPosition(const glm::vec2& pos) {
    transform.position = pos;
}

void gameObject::setPosition(float x, float y) {
    transform.position = { x, y };
}

void gameObject::move(float dx, float dy) {
    transform.position.x += dx;
    transform.position.y += dy;
}

void gameObject::move(const glm::vec2& offset) {
    transform.position += offset;
}