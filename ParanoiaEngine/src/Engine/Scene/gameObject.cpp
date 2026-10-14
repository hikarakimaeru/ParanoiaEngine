#include "gameObject.h"
#include "../Graphics/batchRenderer.h"

gameObject::gameObject(const std::string& name, Texture* texture)
    : name(name), texture(texture) {
}

void gameObject::update(float dt) {
    if (!playing) return;
    
    timer += dt;
    float frameTime = 1.0f / current.fps;

    while (timer >= frameTime) {
        timer -= frameTime;
        frame++;
    }

    frame %= current.count;

    setFrame(current.startCol + frame, current.row);
}

void gameObject::draw(batchRenderer& renderer) {
    renderer.drawQuad(transform.position, transform.size, { 1.0f, 1.0f, 1.0f, 1.0f }, uvMin , uvMax);
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

void gameObject::setSheet(int tw, int th, int fw, int fh) {
    texW = tw;
    texH = th;
    frameW = fw;
    frameH = fh;
}

void gameObject::setSheetGrid(int cols, int rows) {
    texW = texture->width;
    texH = texture->height;
    frameW = texW / cols;
    frameH = texH / rows;
}

void gameObject::setFrame(int col, int row) {
    uvMin = { col * frameW / (float)texW, row * frameH / (float)texH };
    uvMax = { (col + 1) * frameW / (float)texW, (row + 1) * frameH / (float)texH };
}

void gameObject::play(const Animation& a) {
    if (playing && current.row == a.row && current.startCol == a.startCol) return;
    current = a;
    playing = true;
    timer = 0.0f;
    frame = 0;
    setFrame(a.startCol, a.row);
}