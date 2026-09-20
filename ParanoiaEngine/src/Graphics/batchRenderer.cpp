#include "batchRenderer.h"
#include <glm/gtc/type_ptr.hpp>
#include "texture.h"

batchRenderer::~batchRenderer() {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
}

void batchRenderer::init(int width, int height) {
    glViewport(0, 0, width, height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    vertices_buffer.resize(MAX_VERTICES);
    indices.resize(MAX_QUADS * 6);

    shader_program = std::make_unique<ShaderProgram>("Shaders/basic.vert", "Shaders/basic.frag");

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    
    uint32_t offset = 0;
    for (size_t i = 0; i < indices.size(); i += 6) {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;

        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;

        offset += 4;
    }

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void batchRenderer::beginBatch(const Camera& camera) {
    currentCamera = &camera;
    buffer_ptr = vertices_buffer.data();
}

void batchRenderer::drawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color) {
    if (buffer_ptr == nullptr || currentCamera == nullptr) return;

    if (static_cast<size_t>(buffer_ptr - vertices_buffer.data()) >= MAX_VERTICES) {
        flush();
        beginBatch(*currentCamera);
    }

    float x = position.x;
    float y = position.y;
    float w = size.x;
    float h = size.y;

    glm::vec2 texCoords[4] = {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f }
    };

    buffer_ptr->position = { x, y };
    buffer_ptr->color = color;
    buffer_ptr->texCoords = texCoords[0];
    buffer_ptr++;

    buffer_ptr->position = { x + w, y };
    buffer_ptr->color = color;
    buffer_ptr->texCoords = texCoords[1];
    buffer_ptr++;

    buffer_ptr->position = { x + w, y + h };
    buffer_ptr->color = color;
    buffer_ptr->texCoords = texCoords[2];
    buffer_ptr++;

    buffer_ptr->position = { x, y + h };
    buffer_ptr->color = color;
    buffer_ptr->texCoords = texCoords[3];
    buffer_ptr++;
}

void batchRenderer::flush() {
    if (buffer_ptr == nullptr) return;

    size_t vertices_count = buffer_ptr - vertices_buffer.data();

    if (vertices_count == 0) {
        return;
    }

    shader_program->use();

    shader_program->setMatrix4("uViewProjection", currentCamera->getViewProjectionMatrix());

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_count * sizeof(Vertex), vertices_buffer.data());

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>((vertices_count / 4) * 6), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);

    buffer_ptr = vertices_buffer.data();
}