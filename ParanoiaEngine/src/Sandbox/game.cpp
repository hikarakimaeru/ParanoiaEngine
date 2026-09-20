#include "Game.h"
#include "../Engine/Core/time.h"

#include <SDL3/SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include <iostream>

Game::Game()
    : window("ParanoiaEngine - Game Sandbox", 800, 600),
    camera(0.0f, 800.0f, 600.0f, 0.0f),
    playerTexture("assets/HeroKnight_Idle_0.png")
{
    // 1. Инициализация рендерера
    renderer.init(800, 600);

    // 2. Инициализация ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForOpenGL(window.getWin(), window.getGl_Context());
    ImGui_ImplOpenGL3_Init("#version 330");

    // 3. Создаем объекты сцены
    player = new gameObject("Player", &playerTexture);
    player->setPosition(100.0f, 100.0f);
    player->transform.size = { 200.0f, 150.0f };
    scene.addObject(player);

    enemy = new gameObject("Enemy", &playerTexture);
    enemy->setPosition(400.0f, 300.0f);
    enemy->transform.size = { 100.0f, 100.0f };
    scene.addObject(enemy);
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
        render();

        SDL_GL_SwapWindow(window.getWin());
    }
}

void Game::processInput(float dt) {
    ImGuiIO& io = ImGui::GetIO();
    float speed = 300.0f;

    // Управляем игроком, только если мышь/клавиатура не зажаты в окне ImGui
    if (!io.WantCaptureKeyboard) {
        if (input.isHeld(SDL_SCANCODE_W)) player->move(0.0f, -speed * dt);
        if (input.isHeld(SDL_SCANCODE_S)) player->move(0.0f, speed * dt);
        if (input.isHeld(SDL_SCANCODE_A)) player->move(-speed * dt, 0.0f);
        if (input.isHeld(SDL_SCANCODE_D)) player->move(speed * dt, 0.0f);
    }
}

void Game::update(float dt) {
    scene.update(dt);

    // Камера центрируется на игроке
    camera.setPosition(glm::vec3(
        player->getPosition().x - (800.0f / 2.0f),
        player->getPosition().y - (600.0f / 2.0f),
        0.0f
    ));
}

void Game::render() {
    glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Engine Inspector");
    ImGui::Text("Player Controls");
    ImGui::DragFloat2("Player Pos", &player->transform.position.x, 1.0f);
    ImGui::DragFloat("Player Rotation", &player->transform.rotation, 1.0f);
    ImGui::Separator();
    ImGui::Text("Enemy Controls");
    ImGui::DragFloat2("Enemy Pos", &enemy->transform.position.x, 1.0f);
    ImGui::End();

    scene.draw(renderer, camera);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}