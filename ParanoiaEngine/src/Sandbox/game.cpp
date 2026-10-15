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

Game::Game()
    : window("ParanoiaEngine - Game Sandbox", 800, 600),
    camera(0.0f, 800.0f, 600.0f, 0.0f),
    playerTexture("assets/reaper_blade_2.png"),
    enemyTexture("assets/dotd_skeletonsheet_2.png")
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
    player->setPosition(100.0f, 100.0f);
    player->setSheetGrid(3, 4);
    player->transform.size = { player->frameW * 1.0f, player->frameH * 1.0f };

    scene.addObject(player);
    
    float enemyX = 400.0f;
    float enemyY = 300.0f;

    for (int i = 0; i < 1; i++) {
        gameObject* skeleton = EntityFactory::createEnemy(scene, "Skeleton", &enemyTexture, { enemyX, enemyY });
        skeleton->setSheetGrid(12, 8);
        skeleton->transform.size = { skeleton->frameW * 1.0f, skeleton->frameH * 1.0f };
        skeleton->play(skeletonWalkRight);

        enemies.push_back(skeleton);
        enemyX += 50;
        enemyY += 50;
    }
}

Game::~Game() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
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

        hitCooldown -= dt;
        for (gameObject* goblin : enemies) {
            if (hitCooldown <= 0.0f && Collider::checkCollision(*player, *goblin)) {
                //Collider::resolveCollision(*player, *goblin);
                playerHp -= 10;
                hitCooldown = 1.0f;
                std::cout << "Hit!" << std::endl;
            }
        }
        if (playerHp <= 0) {
            playerHp = 100;
            player->setPosition(100.0f, 100.0f); // простой рестарт
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
        if (input.isHeld(SDL_SCANCODE_W)) { player->move(0.0f, -speed * dt); moving = true; }
        if (input.isHeld(SDL_SCANCODE_S)) { player->move(0.0f, speed * dt); moving = true; }
        if (input.isHeld(SDL_SCANCODE_A)) { player->move(-speed * dt, 0.0f); moving = true; }
        if (input.isHeld(SDL_SCANCODE_D)) { player->move(speed * dt, 0.0f); moving = true; }

        if (input.isHeld(SDL_SCANCODE_SPACE)) {
            float px = player->transform.position.x + player->transform.size.x / 2;
            float py = player->transform.position.y + player->transform.size.y / 2;

            for (size_t i = 0; i < enemies.size();) {
                gameObject* e = enemies[i];

                float dx = e->transform.position.x + e->transform.size.x / 2 - px;
                float dy = e->transform.position.y + e->transform.size.y / 2 - py;

                float dist = std::sqrt(dx * dx + dy * dy);

                if (dist < attackRadius) {
                    scene.removeObject(e);
                    enemies.erase(enemies.begin() + i);
                }
                else {
                    i++;
                }
            }
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

    camera.setPosition(glm::vec3(
        player->getPosition().x - (800.0f / 2.0f),
        player->getPosition().y - (600.0f / 2.0f),
        0.0f
    ));

    for (gameObject* e : enemies) {
        float dx = player->transform.position.x - e->transform.position.x;
        float dy = player->transform.position.y - e->transform.position.y;

        float len = std::sqrt(dx * dx + dy * dy);
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

void Game::drawHitboxes() {
    if (!showHitboxes) return;

    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    glm::vec3 camPos = camera.getPosition();         

    auto drawBox = [&](gameObject* obj, ImU32 color) {
        float x = obj->transform.position.x - camPos.x;
        float y = obj->transform.position.y - camPos.y;
        float w = obj->transform.size.x;
        float h = obj->transform.size.y;
        dl->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 0.0f, 0, 2.0f);
        };

    drawBox(player, IM_COL32(0, 255, 0, 255));
    for (gameObject* e : enemies)
        drawBox(e, IM_COL32(255, 0, 0, 255));
}


void Game::render() {
    glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Engine Inspector");
    ImGui::Checkbox("Show Hitboxes", &showHitboxes);
    ImGui::Text("Player Controls");
    ImGui::DragFloat2("Player Pos", &player->transform.position.x, 1.0f);
    if (!enemies.empty()) {
        ImGui::Separator();
        ImGui::Text("Skeleton Controls");
        ImGui::DragFloat2("Skeleton Pos", &enemies[0]->transform.position.x, 1.0f);
    }
    ImGui::Text("HP: %d", playerHp);
    ImGui::End();

    scene.draw(renderer, camera);

    drawHitboxes();
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}