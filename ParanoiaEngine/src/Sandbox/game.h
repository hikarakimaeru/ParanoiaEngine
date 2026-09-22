#pragma once

#include "../Engine/Core/window.h"
#include "../Engine/Core/input.h"
#include "../Engine/Graphics/batchRenderer.h"
#include "../Engine/Graphics/camera.h"
#include "../Engine/Graphics/texture.h"
#include "../Engine/Scene/scene.h"

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void processInput(float dt);
    void update(float dt);
    void render();

    Window window;
    batchRenderer renderer;
    Camera camera;
    Scene scene;
    Input input;

    Texture playerTexture;
    Texture enemyTexture;

    gameObject* player = nullptr;
    std::vector<gameObject*> enemies; 

    bool running = true;
};