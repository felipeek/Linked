#include "TextShader.h"
#include "Display.h"
#include <glm\gtc\matrix_transform.hpp>

TextShader::TextShader(std::string fileName) : Shader(fileName, NULL)
{
	getUniformLocations();
}


TextShader::~TextShader()
{
}

void TextShader::getUniformLocations()
{
	uniformProjection = glGetUniformLocation(shader, "projection");
	uniformText = glGetUniformLocation(shader, "text");
	uniformTextColor = glGetUniformLocation(shader, "textColor");
}

void TextShader::update()
{
	glm::mat4 projection = glm::ortho(0.0f, (float)Display::getCurrentInstance().getWidth(), 0.0f, (float)Display::getCurrentInstance().getHeight());
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, &projection[0][0]);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(uniformText, 0);
}

void TextShader::useShader()
{
	Shader::useShader();
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TextShader::stopShader()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	Shader::stopShader();
}

GLuint TextShader::getUniformTextColorLocation()
{
	return uniformTextColor;
}