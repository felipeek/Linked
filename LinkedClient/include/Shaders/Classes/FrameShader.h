#pragma once
#include "Shader.h"
#include <string>

class FrameShader : public Shader
{
public:
	FrameShader(std::string fileName);
	~FrameShader();
	virtual void update();
private:
	virtual void getUniformLocations();
	GLuint uniform_TexSampler;
};

