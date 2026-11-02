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

    bool showHitboxes = true;
    void drawHitboxes();
    void drawBackground();
    void spawnWaves(float dt);
    void restart();

    Window window;
    batchRenderer renderer;
    Camera camera;
    Scene scene;
    Input input;

    Texture floorTexture;

    Animation playerWalkDown;
    Animation playerWalkLeft;
    Animation playerWalkRight;
    Animation playerWalkUp;
    Animation playerIdle;

    Animation skeletonWalkDown;
    Animation skeletonWalkLeft;
    Animation skeletonWalkRight;
    Animation skeletonWalkUp;
    Animation skeletonIdle;

    Texture playerTexture;
    Texture enemyTexture;
    Texture HPBarTexture;


    gameObject* player = nullptr;
    gameObject* HPBar = nullptr;

    std::vector<gameObject*> enemies; 

    const float TILE_SIZE = 32.0f;
    const float SCREEN_W = 800.0f;
    const float SCREEN_H = 600.0f;

    int n_wave = 0;
    int lastHpStep = -1;
    float waveCooldown = 5.0f;
    float enemySpeed = 80.0f;
    int playerHp = 100;
    float hitCooldown = 0.0f;
    float attackRadius = 80.0f;
    float attackCooldown = 1.5f;
    int kills = 0;
    bool running = true;
};