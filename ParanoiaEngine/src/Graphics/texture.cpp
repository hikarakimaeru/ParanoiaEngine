#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../vendor/stb_image.h"

Texture::Texture(const char* filePath) {
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// stbi_set_flip_vertically_on_load(true);

	int width, height, nr_channels;
	unsigned char* data = stbi_load(filePath, &width, &height, &nr_channels, 0);

	if (data) {
		GLenum format = (nr_channels == 4) ? GL_RGBA : GL_RGB;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}

}

Texture::~Texture() {
	if (id) {
		glDeleteTextures(1, &id);
	}
}

void Texture::bind(unsigned int slot) const {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id);
}
