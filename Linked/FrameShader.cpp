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

void FrameShader::update(Transform* t, Entity* e)
{
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(uniform_TexSampler, 0);
}