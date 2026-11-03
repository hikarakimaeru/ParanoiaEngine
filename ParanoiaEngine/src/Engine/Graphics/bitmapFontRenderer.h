#pragma once
#include <string>
#include <map>
#include "batchRenderer.h"

struct FontChar {
	int id;
	int x, y;
	int width, height;
	int xOffset, yOffset;
	float xAdvance;
};

class BitmapFontRenderer {
private:
	std::map<int, FontChar> fontCharacters;
	int texture_width;
	int texture_height;
	int line_height;
	int base;
	std::string texture_filename;
	unsigned int font_texture_id = 0;
public:
	void load(const std::string& fntPath);
	void renderText(batchRenderer& renderer, const Camera& camera, Texture& tex, const std::string& text, float startX, float startY);
};