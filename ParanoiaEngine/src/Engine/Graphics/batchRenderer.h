#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <array>
#include <memory>
#include "shaderProgram.h"
#include "texture.h"
#include "camera.h"

struct Vertex {
    glm::vec2 position;
    glm::vec4 color;
    glm::vec2 texCoords;
};

class batchRenderer {
private:
    static const size_t MAX_QUADS = 10000;
    static const size_t MAX_VERTICES = MAX_QUADS * 4;

    std::vector<Vertex> vertices_buffer;
    Vertex* buffer_ptr = nullptr;

    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    std::vector<uint32_t> indices;

    const Camera* currentCamera = nullptr;

public:
    std::unique_ptr<ShaderProgram> shader_program;

    batchRenderer() = default;
    ~batchRenderer();

    void init(int width, int height);
    void beginBatch(const Camera& camera);
    void drawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color = glm::vec4(1.0f));
    void flush();
};
