#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>

//#define LETTER_WIDTH 0		// 0 to auto
//#define LETTER_HEIGHT 100	// max height
#define FONT_SIZE 1.0f

class Shader;

enum charPixelSize{
	_12PX = 12,
	_14PX = 14,
	_16PX = 16,
	_18PX = 18
};

struct Character {
	GLuint     TextureID;  // ID handle of the glyph texture
	glm::ivec2 Size;       // Size of glyph
	glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
	GLuint     Advance;    // Offset to advance to next glyph
};

class TextRenderer
{
public:
	TextRenderer(Shader* shader, std::string fontName);
	~TextRenderer();

	void renderText(unsigned char* text, int textSize, GLfloat x, GLfloat y, charPixelSize scale, glm::vec3 color);
	Shader* getShader();
private:
	std::map<unsigned int, Character> chars12px;
	std::map<unsigned int, Character> chars14px;
	std::map<unsigned int, Character> chars16px;
	std::map<unsigned int, Character> chars18px;

	GLuint VAO, VBO, IBO;
	Shader* shader;

	FT_Library ft;
	FT_Face face;

	void initRenderer(std::string fontName, int fontPixelSize, std::map<unsigned int, Character>& charList);
	void genDynamicVAO();
};

