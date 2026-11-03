#include "bitmapFontRenderer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "batchRenderer.h"

void BitmapFontRenderer::load(const std::string& fntPath) {
	std::ifstream file(fntPath);
	std::string line;

	if (!file.is_open()) {
		std::cerr << "Cannot open font path!" << std::endl;
		return;
	}

	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string tag;
		ss >> tag;
		if (tag == "common") {
			std::string pair;
			while (ss >> pair) {
				size_t eqPos = pair.find("=");
				if (eqPos == std::string::npos) {
					continue;
				}
				std::string key = pair.substr(0, eqPos);
				std::string value = pair.substr(eqPos + 1);
				if (key == "scaleW") {
					texture_width = std::stoi(value);
				}
				else if (key == "scaleH") {
					texture_height = std::stoi(value);
				}
				else if (key == "lineHeight") {
					line_height = std::stoi(value);
				}
				else if (key == "base") {
					base = std::stoi(value);
				}
			}
		}else if (tag == "pages" || tag == "page") {
			int pageId = 0;
			std::string pair;

			std::string textureFilename = "";

			while (ss >> pair) {
				size_t eqPos = pair.find("=");
				if (eqPos == std::string::npos) {
					continue;
				}
				std::string key = pair.substr(0, eqPos);
				std::string value = pair.substr(eqPos + 1);

				if (key == "id") {
					pageId = std::stoi(value);
				}
				else if (key == "file") {
					if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
						textureFilename = value.substr(1, value.size() - 2);
					}else {
						textureFilename = value;
					}
				}
			}
			texture_filename = textureFilename;
		}
		else if (tag == "char") {
			FontChar ch{};
			std::string pair;
			int id = 0;

			while (ss >> pair) {
				size_t eqPos = pair.find("=");
				if (eqPos == std::string::npos) {
					continue;
				}

				std::string key = pair.substr(0, eqPos);
				std::string value = pair.substr(eqPos + 1);

				if (key == "id") id = std::stoi(value);
				else if (key == "x") ch.x = std::stoi(value);
				else if (key == "y") ch.y = std::stoi(value);
				else if (key == "width") ch.width = std::stoi(value);
				else if (key == "height") ch.height = std::stoi(value);
				else if (key == "xoffset") ch.xOffset = std::stoi(value);
				else if (key == "yoffset") ch.yOffset = std::stoi(value);
				else if (key == "xadvance") ch.xAdvance = std::stof(value);
			}

			ch.id = id;
			fontCharacters[id] = ch;
		}
	}
}

void BitmapFontRenderer::renderText(batchRenderer& renderer, const Camera& camera, Texture& tex, const std::string& text, float startX, float startY) {
	float cursorX = startX;
	float cursorY = startY;

	renderer.beginBatch(camera);
	tex.bind(0);

	for (char c : text) {
		int id = static_cast<unsigned char>(c);

		if (c == '\n') {
			cursorX = startX;
			cursorY += line_height;
			continue;
		}

		auto it = fontCharacters.find(id);
		if (it == fontCharacters.end()) {
			continue;
		}

		const FontChar& ch = it->second;

		float xpos = cursorX + ch.xOffset;
		float ypos = cursorY + ch.yOffset;

		float w = static_cast<float>(ch.width);
		float h = static_cast<float>(ch.height);

		glm::vec2 uvMin = { (float)ch.x / texture_width, (float)ch.y / texture_height };
		glm::vec2 uvMax = { (float)(ch.x + ch.width) / texture_width, (float)(ch.y + ch.height) / texture_height };

		glm::vec4 textColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		renderer.drawQuad({ xpos, ypos }, { w, h }, textColor, uvMin, uvMax);

		cursorX += ch.xAdvance;
	}

	renderer.flush();
}