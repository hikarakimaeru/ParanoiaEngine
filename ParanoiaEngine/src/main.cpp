#define SDL_MAIN_HANDLED

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <iostream>
#include <format>

// Third-party
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

// Engine Core
#include "Core/window.h"
#include "Core/time.h"
#include "Core/input.h"

// Engine Graphics
#include "Graphics/texture.h"
#include "Graphics/camera.h"
#include "Graphics/batchRenderer.h" // 1. Подключаем новый Batch Renderer

// Engine Scene System
#include "Scene/gameObject.h"
#include "Scene/Scene.h"

int main(int argc, char* argv[]) {
    std::cout << std::format("Welcome to ParanoiaEngine!\n");

    int width = 800;
    int height = 600;

    Window window("ParanoiaEngine", width, height);

    // 2. Инициализируем batchRenderer вместо старого Renderer
    batchRenderer renderer;
    renderer.init(width, height);

    // ImGui Initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForOpenGL(window.getWin(), window.getGl_Context());
    ImGui_ImplOpenGL3_Init("#version 330");

    // Assets
    // (Массив vertices и объект Mesh больше НЕ нужны!)
    Texture texture("assets/HeroKnight_Idle_0.png");
    Camera camera(0.0f, (float)width, (float)height, 0.0f);

    // Создаем сцену и добавляем объекты
    Scene scene;

    gameObject* player = new gameObject("Player", &texture);
    player->setPosition(100.0f, 100.0f);
    player->transform.size = { 200.0f, 150.0f };
    scene.addObject(player);

    gameObject* enemy = new gameObject("Enemy", &texture);
    enemy->setPosition(400.0f, 300.0f);
    enemy->transform.size = { 100.0f, 100.0f };
    scene.addObject(enemy);

    float speed = 300.0f;

    Input input;
    SDL_Event event;
    bool running = true;

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

        // Управление игроком
        if (!io.WantCaptureKeyboard) {
            if (input.isHeld(SDL_SCANCODE_W)) player->move(0.0f, -speed * dt);
            if (input.isHeld(SDL_SCANCODE_S)) player->move(0.0f, speed * dt);
            if (input.isHeld(SDL_SCANCODE_A)) player->move(-speed * dt, 0.0f);
            if (input.isHeld(SDL_SCANCODE_D)) player->move(speed * dt, 0.0f);
        }

        // Обновляем логику сцены
        scene.update(dt);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // Инспектор ImGui
        ImGui::Begin("Engine Inspector");
        ImGui::Text("Player Controls");
        ImGui::DragFloat2("Player Pos", &player->transform.position.x, 1.0f);
        ImGui::DragFloat("Player Rotation", &player->transform.rotation, 1.0f);
        ImGui::Separator();
        ImGui::Text("Enemy Controls");
        ImGui::DragFloat2("Enemy Pos", &enemy->transform.position.x, 1.0f);
        ImGui::End();

        // Камера следит за игроком
        camera.setPosition(glm::vec3(player->getPosition().x - ((float)width / 2),
            player->getPosition().y - ((float)height / 2), 0.0f));

        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene.draw(renderer, camera);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window.getWin());
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    return 0;
}