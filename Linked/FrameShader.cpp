#include "FrameShader.h"

FrameShader::FrameShader(std::string fileName) : Shader(fileName, NULL)
{
	getUniformLocations();
}

FrameShader::~FrameShader()
{
}

void FrameShader::getUniformLocations()
{
	uniform_TexSampler = glGetUniformLocation(shader, "TextureSampler");
}

void FrameShader::update()
{
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(uniform_TexSampler, 0);
}