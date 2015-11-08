#pragma once
#include "Shader.h"
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>

class FontShader : public Shader
{
public:
	FontShader(std::string);
	~FontShader();

	virtual void update();

	

	glm::vec2 clipTL;
	glm::vec2 clipBR;

private:
	virtual void getUniformLocations();

	GLuint uniformProjection;
	GLuint uniformTextTexture;
	GLuint uniformTextColor;
	GLuint uniformModel;

	GLuint uniformClipTopLeft;
	GLuint uniformClipBotRight;

	glm::vec4 m_textColor;
public:
	/* getters and setters */
	inline glm::vec4& getColor() { return this->m_textColor; }
	inline void setTextColor(const glm::vec4& color) { this->m_textColor = color; }
};

