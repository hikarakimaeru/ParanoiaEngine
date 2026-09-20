#pragma once

#include <glad/glad.h>

class Texture {
public:
	Texture(const char* filePath);
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	void bind(unsigned int slot) const;

private:
	GLuint id = 0;
};