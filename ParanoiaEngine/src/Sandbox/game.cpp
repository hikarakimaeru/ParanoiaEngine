#include "Game.h"
#include "../Engine/Core/time.h"

#include <SDL3/SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include "../Engine/Core/collider.h"
#include "../Engine/Scene/entityFactory.h"
#include <cmath>
#include <random>

Game::Game()
    : window("ParanoiaEngine - Game Sandbox", 800, 600),
    camera(0.0f, 800.0f, 600.0f, 0.0f),
    playerTexture("assets/reaper_blade_2.png"),
    enemyTexture("assets/dotd_skeletonsheet_2.png"),
    floorTexture("assets/block.png"),
    HPBarTexture("assets/HPBar.png"),
    particleTexture("assets/particle.png")
{
    renderer.init(800, 600);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForOpenGL(window.getWin(), window.getGl_Context());
    ImGui_ImplOpenGL3_Init("#version 330");

    playerWalkDown = { 0, 0, 3, 8.0f };
    playerWalkLeft = { 1, 0, 3, 8.0f };
    playerWalkRight = { 2, 0, 3, 8.0f };
    playerWalkUp = { 3, 0, 3, 8.0f };
    playerIdle = { 0, 1, 1, 8.0f };

    skeletonWalkDown = { 0, 0, 3, 8.0f };
    skeletonWalkLeft = { 1, 0, 3, 8.0f };
    skeletonWalkRight = { 2, 0, 3, 8.0f };
    skeletonWalkUp = { 3, 0, 3, 8.0f };
    skeletonIdle = { 0, 1, 1, 8.0f };

    player = new gameObject("Player", &playerTexture);
    player->setPosition(960.0f, 960.0f);
    player->setSheetGrid(3, 4);
    player->transform.size = { player->frameW * 1.0f, player->frameH * 1.0f };

    scene.addObject(player);   

    HPBar = new gameObject("HP bar", &HPBarTexture);
    HPBar->setSheetGrid(7, 15);
    HPBar->transform.size = { HPBar->frameW * 4.0f, HPBar->frameH * 4.0f };
    scene.addObject(HPBar);
}

Game::~Game() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void Game::drawBackground() {
    glm::vec3 cam = camera.getPosition();
    glm::vec2 uvMin = { cam.x / TILE_SIZE, cam.y / TILE_SIZE };
    glm::vec2 uvMax = { uvMin.x + SCREEN_W / TILE_SIZE, uvMin.y + SCREEN_H / TILE_SIZE };

    renderer.beginBatch(camera);
    floorTexture.bind(0);
    renderer.drawQuad({ cam.x, cam.y }, { SCREEN_W, SCREEN_H }, { 1, 1, 1, 1 }, uvMin, uvMax);
    renderer.flush();
}


void Game::run() {
    SDL_Event event;

    while (running) {
        Time::update();
        float dt = Time::getDeltaTime();

        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            input.processEvent(event);
        }
        
        processInput(dt);
        update(dt);

        spawnWaves(dt);

        hitCooldown -= dt;
        for (gameObject* goblin : enemies) {
            if (hitCooldown <= 0.0f && Collider::checkCollision(*player, *goblin)) {
                //Collider::resolveCollision(*player, *goblin);
                playerHp -= 10;
                hitCooldown = 1.0f;
                std::cout << "Hit!" << std::endl;
                particles.emit(
                    { player->transform.position.x + player->transform.size.x / 2.0f,
                      player->transform.position.y + player->transform.size.y / 2.0f },
                    26, { 0.0f, 0.0f, 0.0f });
            }
        }
        if (playerHp <= 0) {
            playerHp = 100;
            restart();
        }
        render();

        SDL_GL_SwapWindow(window.getWin());
    }
}

void Game::processInput(float dt) {
    ImGuiIO& io = ImGui::GetIO();
    float speed = 300.0f;
    bool moving = false;

    if (!io.WantCaptureKeyboard) {
        glm::vec2 dir(0.0f, 0.0f);

        if (input.isHeld(SDL_SCANCODE_W)) { dir.y -= 1.0f; }
        if (input.isHeld(SDL_SCANCODE_S)) { dir.y += 1.0f; }
        if (input.isHeld(SDL_SCANCODE_A)) { dir.x -= 1.0f; }
        if (input.isHeld(SDL_SCANCODE_D)) { dir.x += 1.0f; }

        if (dir.x != 0.0f || dir.y != 0.0f) {
            dir = glm::normalize(dir);
            player->move(dir.x * speed * dt, dir.y * speed * dt);
            moving = true;
        }

        attackCooldown -= dt;
        if (attackCooldown <= 0.0f && input.isHeld(SDL_SCANCODE_SPACE)) {
            float px = player->transform.position.x + player->transform.size.x / 2;
            float py = player->transform.position.y + player->transform.size.y / 2;

            for (int i = 0; i < 16; i++) {
                float angle = i * 6.2832f / 16.0f;
                particles.emit(
                    { px + std::cos(angle) * attackRadius, py + std::sin(angle) * attackRadius },
                    4, { 0.0f, 255.0f, 0.0f });
            }

            for (size_t i = 0; i < enemies.size();) {
                gameObject* e = enemies[i];

                float dx = e->transform.position.x + e->transform.size.x / 2 - px;
                float dy = e->transform.position.y + e->transform.size.y / 2 - py;

                float dist = std::sqrt(dx * dx + dy * dy);

                if (dist < attackRadius + e->transform.size.x / 2.0f) {
                    particles.emit(
                        { e->transform.position.x + e->transform.size.x / 2.0f,
                          e->transform.position.y + e->transform.size.y / 2.0f },
                        26, { 1.0f, 0.3f, 0.3f });
                    scene.removeObject(e);
                    kills++;
                    enemies.erase(enemies.begin() + i);
                }
                else {
                    i++;
                }
            }
            std::printf("attack!\n");
            attackCooldown = 1.5f;
        }

        if (input.isHeld(SDL_SCANCODE_A))      player->play(playerWalkLeft);
        else if (input.isHeld(SDL_SCANCODE_D)) player->play(playerWalkRight);
        else if (input.isHeld(SDL_SCANCODE_W)) player->play(playerWalkUp);
        else if (input.isHeld(SDL_SCANCODE_S)) player->play(playerWalkDown);
    }

    if (!moving) player->play(playerIdle);
}

void Game::update(float dt) {
    scene.update(dt);
    particles.update(dt);

    camera.setPosition(glm::vec3(
        player->getPosition().x - (SCREEN_W / 2.0f),
        player->getPosition().y - (SCREEN_H / 2.0f),
        0.0f
    ));

    glm::vec3 cam = camera.getPosition();
    HPBar->setPosition(cam.x + 20.0f, cam.y + SCREEN_H - 75.0f);

    int hp = playerHp;
    if (hp < 0) hp = 0;
    if (hp > 100) hp = 100;

    int step = (100 - hp) / 17;

    if (step != lastHpStep) {
        HPBar->play({ 0, 1 + step, 1, 1.0f });
        lastHpStep = step;
    }

    for (gameObject* e : enemies) {
        float dx = player->transform.position.x - e->transform.position.x;
        float dy = player->transform.position.y - e->transform.position.y;

        float len = std::sqrt(dx * dx + dy * dy);
        if (len < 0.001f) continue;
        if (std::abs(dx) > std::abs(dy)) {
            if (dx / len > 0) {
                e->play(skeletonWalkRight);
            }
            else if (dx / len < 0) {
                e->play(skeletonWalkLeft);
            }
        }
        else {
            if (dy / len > 0) {
                e->play(skeletonWalkDown);
            }
            else if (dy / len < 0) {
                e->play(skeletonWalkUp);
            }
        }
        e->transform.position.x += dx / len * enemySpeed * dt;
        e->transform.position.y += dy / len * enemySpeed * dt;
    }
}


void Game::render() {
    glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Engine Inspector");
    ImGui::Text("Kills: %d", kills);
    ImGui::End();

    drawBackground();
    scene.draw(renderer, camera);

    particles.draw(renderer, camera, particleTexture);
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Game::spawnWaves(float dt) {
    std::random_device rd;
    std::mt19937 gen(rd());

    glm::vec3 cam = camera.getPosition();

    std::uniform_real_distribution<float> sideDist(0, 1);
    std::uniform_real_distribution<float> yDist(0, SCREEN_H);

    waveCooldown -= dt;
    if (waveCooldown <= 0.0f) {
        for (int i = 0; i < 3 + n_wave; i++) {
            float side = sideDist(gen);
            float x = 0.0f;

            if (side < 0.5f) {
                x = cam.x - 100.0f;
            }
            else {
                x = cam.x + 900.0f;
            }

            int y = cam.y + yDist(gen);

            gameObject* skeleton = EntityFactory::createEnemy(scene, "Skeleton", &enemyTexture, { x, y });
            skeleton->setSheetGrid(12, 8);
            skeleton->transform.size = { skeleton->frameW * 1.0f, skeleton->frameH * 1.0f };
            skeleton->play(skeletonWalkRight);
            enemies.push_back(skeleton);
        }
        waveCooldown = 5.0f;
        n_wave++;
    }
}

void Game::restart() {
    playerHp = 100;
    player->setPosition(960.0f, 960.0f);
    for (gameObject* e : enemies) {
        scene.removeObject(e);
    }
    enemies.clear();
    waveCooldown = 5.0f;
    n_wave = 0;
    kills = 0;
}