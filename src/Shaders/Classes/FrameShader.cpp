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
	uniform_TexSampler = glGetUniformLocation(shader, "screenTexture");
}

void FrameShader::update()
{
	glUniform1i(uniform_TexSampler, 0);
}