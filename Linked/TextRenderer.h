#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>

#define LETTER_WIDTH 0		// 0 to auto
#define LETTER_HEIGHT 100	// max height

class Shader;

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

	void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
private:
	std::map<GLchar, Character> Characters;

	GLuint VAO, VBO;
	Shader* shader;

	FT_Library ft;
	FT_Face face;

	void initRenderer(std::string fontName);
	void genDynamicVAO();
};

