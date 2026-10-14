#pragma once

#include <string>
#include <glm/glm.hpp>

#include "../Core/transform.h"
#include "../Graphics/texture.h"

class batchRenderer;

struct Animation {
    int row;
    int startCol;
    int count;
    float fps;
};

class gameObject {
public:
    std::string name;
    Texture* texture = nullptr;
    Transform transform;

    glm::vec2 uvMin = { 0.0f, 0.0f };
    glm::vec2 uvMax = { 1.0f, 1.0f };

    int texW = 1, texH = 1;
    int frameW = 1, frameH = 1;

    void setSheet(int tw, int th, int fw, int fh);
    void setFrame(int col, int row);

    Animation current{};
    bool playing = false;
    float timer = 0.0f;
    int frame = 0;

    void play(const Animation& a);

    gameObject(const std::string& name = "GameObject", Texture* texture = nullptr);
    virtual ~gameObject() = default;

    virtual void update(float dt);

    void draw(batchRenderer& renderer);

    void setSheetGrid(int cols, int rows);

    glm::vec2 getPosition() const;
    void setPosition(const glm::vec2& pos);
    void setPosition(float x, float y);
    void move(float dx, float dy);
    void move(const glm::vec2& offset);
};