#include "shaderProgram.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>


ShaderProgram::ShaderProgram(const std::filesystem::path& vertPath, const std::filesystem::path& fragPath) {
    std::string vertex_shader_source = loadShaderSource(vertPath);
    std::string fragment_shader_source = loadShaderSource(fragPath);

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertSrc = vertex_shader_source.c_str();
    glShaderSource(vertex_shader, 1, &vertSrc, nullptr);
    glCompileShader(vertex_shader);
    checkCompileErrors(vertex_shader, "VERTEX");

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragSrc = fragment_shader_source.c_str();
    glShaderSource(fragment_shader, 1, &fragSrc, nullptr);
    glCompileShader(fragment_shader);
    checkCompileErrors(fragment_shader, "FRAGMENT");

    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, fragment_shader);
    glLinkProgram(program_id);
    checkCompileErrors(program_id, "PROGRAM");

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

ShaderProgram::~ShaderProgram() {
    if (program_id) {
        glDeleteProgram(program_id);
    }
}

void ShaderProgram::use() const {
    glUseProgram(program_id);
}

void ShaderProgram::unuse() const {
    glUseProgram(0);
}

GLuint ShaderProgram::getProgramId() const {
    return program_id;
}

std::string ShaderProgram::loadShaderSource(const std::filesystem::path& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Can't open shader file: " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void ShaderProgram::checkCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void ShaderProgram::setInt(const std::string& name, int value) const {
    GLint location = glGetUniformLocation(program_id, name.c_str());
    glUniform1i(location, value);
}

void ShaderProgram::setMatrix4(const std::string& name, const glm::mat4& matrix) const {
    GLint location = glGetUniformLocation(program_id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}