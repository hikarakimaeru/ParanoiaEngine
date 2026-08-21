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
#include "Core/Window.h"
#include "Core/Time.h"
#include "Core/Input.h"

// Engine Graphics
#include "Graphics/Texture.h"
#include "Graphics/Mesh.h"
#include "Graphics/ShaderProgram.h"

int main(int argc, char* argv[]) {
    std::cout << std::format("Welcome to the engine!\n");

    Window window("ParanoiaEngine", 800, 600);

    // ImGui Initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForOpenGL(window.getWin(), window.getGl_Context());
    ImGui_ImplOpenGL3_Init("#version 330");

    // Geometry & Assets
    float vertices[] = {
        1.0f, 0.0f,  1.0f, 0.0f, // верхний правый
        1.0f, 1.0f,  1.0f, 1.0f, // нижний правый
        0.0f, 1.0f,  0.0f, 1.0f, // нижний левый
        0.0f, 0.0f,  0.0f, 0.0f  // верхний левый
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    Mesh mesh(vertices, indices);
    Texture texture("assets/HeroKnight_Idle_0.png");
    ShaderProgram shader_program("Shaders/basic.vert", "Shaders/basic.frag");

    // Shader Uniforms Setup
    shader_program.use();
    GLuint projection_loc = glGetUniformLocation(shader_program.getProgramId(), "projection");
    GLuint transform_loc = glGetUniformLocation(shader_program.getProgramId(), "transform");
    GLuint tex_loc = glGetUniformLocation(shader_program.getProgramId(), "u_Texture");
    glUniform1i(tex_loc, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // State Variables
    float position[2] = { 100.0f, 100.0f };
    float size[2] = { 200.0f, 150.0f };
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

        // Gameplay Update
        if (input.isHeld(SDL_SCANCODE_W)) position[1] -= speed * dt;
        if (input.isHeld(SDL_SCANCODE_S)) position[1] += speed * dt;
        if (input.isHeld(SDL_SCANCODE_A)) position[0] -= speed * dt;
        if (input.isHeld(SDL_SCANCODE_D)) position[0] += speed * dt;

        // UI Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Engine Inspector");
        ImGui::SliderFloat2("Position (X, Y)", position, 0.0f, 800.0f);
        ImGui::SliderFloat2("Size (X, Y)", size, 10.0f, 500.0f);
        ImGui::End();

        // Render
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_program.use();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position[0], position[1], 0.0f));
        transform = glm::scale(transform, glm::vec3(size[0], size[1], 1.0f));
        glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));

        texture.bind(0);
        mesh.bind();
        glDrawElements(GL_TRIANGLES, mesh.getIndexCount(), GL_UNSIGNED_INT, nullptr);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window.getWin());
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    return 0;
}