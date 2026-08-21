#include "mesh.h"

Mesh::Mesh(std::span<const float> vertices, std::span<const unsigned int> indices) : index_count(static_cast<GLsizei>(indices.size())) {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);

	GLsizei stride = sizeof(float) * 4;
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 2));
	glEnableVertexAttribArray(1);
}

Mesh::~Mesh() {
	if (EBO) {
		glDeleteBuffers(1, &EBO);
	}
	if (VBO) {
		glDeleteBuffers(1, &VBO);
	}
	if (VAO) {
		glDeleteVertexArrays(1, &VAO);
	}
}

void Mesh::bind() const {
	glBindVertexArray(VAO);
}

void Mesh::unbind() const {
	glBindVertexArray(0);
}

GLuint Mesh::getVAO() const {
	return VAO;
}
GLuint Mesh::getVBO() const {
	return VBO;
}
GLsizei Mesh::getIndexCount() const {
	return index_count;
}

