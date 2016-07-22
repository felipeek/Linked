#pragma once

#include "external/freetypegl/freetype-gl.h"
//#include "external/freetypegl/texture-font.h"

#include <glm/glm.hpp>
#include <string>

class FontShader;
class Mesh;

struct TextInfo
{
	int num_rows;
	float width;
	float height;
	glm::vec2 start_position;
	glm::vec2 end_position;
};

class FontRenderer
{
public:
	FontRenderer(float fontSize, int textureQuality);
	~FontRenderer();
	TextInfo RenderText(std::string text, float xPos, float yPos, float pixelWidthLimit, const glm::vec4 color, FontShader* shader, bool wordFormat);
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

