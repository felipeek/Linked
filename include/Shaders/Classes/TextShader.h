#pragma once
#include <string>
#include "Shader.h"

class TextShader : public Shader
{
public:
	TextShader(std::string fileName);
	~TextShader();

	void update();
	void useShader();
	void stopShader();

	GLuint getUniformTextColorLocation();
private:
	GLuint uniformProjection;
	GLuint uniformText;
	GLuint uniformTextColor;

	void getUniformLocations();
};
