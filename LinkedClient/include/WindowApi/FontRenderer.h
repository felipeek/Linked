#pragma once

#include "external/freetypegl/freetype-gl.h"

#include <glm/glm.hpp>

class FontShader;
class Mesh;

class FontRenderer
{
public:
	FontRenderer(float fontSize, int textureQuality);
	~FontRenderer();
	int renderText(const unsigned char* text, int textSize, const glm::vec2& position, float rightLimit, float bottomLimit, const glm::vec4 color, FontShader* shader);
private:
	ftgl::texture_atlas_t* atlas;
	ftgl::texture_font_t* font;
	unsigned int* data;
	float fontSize;
	float ascender;
	Mesh* fontMesh;
public:
	inline float getFontSize() const { return this->fontSize; }
	inline float getAscender() const { return this->ascender; }
};

