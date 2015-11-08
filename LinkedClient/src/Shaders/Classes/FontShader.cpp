#include "FontShader.h"

#include <glm\gtc\matrix_transform.hpp>

#include <fstream>
#include <iostream>

FontShader::FontShader(std::string filename) : Shader(filename, nullptr)
{
	getUniformLocations();
}

FontShader::~FontShader()
{
}

void FontShader::getUniformLocations()
{
	uniformTextTexture = glGetUniformLocation(shader, "textTexture");
	uniformTextColor = glGetUniformLocation(shader, "textColor");

	uniformClipTopLeft = glGetUniformLocation(shader, "clipTL");
	uniformClipBotRight = glGetUniformLocation(shader, "clipBR");
}

void FontShader::update()
{
	glUniform4fv(uniformTextColor, 1, &m_textColor[0]);
	glUniform1i(uniformTextTexture, 0);
	glUniform2fv(uniformClipTopLeft, 1, &clipTL[0]);
	glUniform2fv(uniformClipBotRight, 1, &clipBR[0]);
}