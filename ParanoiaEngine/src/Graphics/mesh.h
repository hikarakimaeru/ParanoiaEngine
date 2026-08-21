#pragma once

#include <glad/glad.h>
#include <span>

class Mesh {
public:
	Mesh(std::span<const float> vertices, std::span<const unsigned int> indices);
		
	~Mesh();

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	void bind() const;

	void unbind() const;

	GLuint getVAO() const;
	GLuint getVBO() const;
	GLsizei getIndexCount() const;

private:
	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint EBO = 0;
	GLsizei index_count = 0;
};
