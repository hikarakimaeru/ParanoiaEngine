#pragma once

#include <glad/glad.h>
#include <filesystem>
#include <string>

class ShaderProgram {
public:
    ShaderProgram(const std::filesystem::path& vertPath, const std::filesystem::path& fragPath);
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    void use() const;
    void unuse() const;

    GLuint getProgramId() const;

private:
    GLuint program_id = 0;

    std::string loadShaderSource(const std::filesystem::path& filePath);
    void checkCompileErrors(GLuint shader, const std::string& type);
};