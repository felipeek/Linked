#pragma once
#include "Shader.h"

class Light;
class Entity;

class GUIShader : public Shader
{
public:
	GUIShader(std::string fileName);
	~GUIShader();
	virtual void update();
	virtual void useShader();
	virtual void stopShader();
private:
	virtual void getUniformLocations();
	GLuint uniform_TexSampler;
	GLuint uniform_Model;
	GLuint uniform_textureNumRows;
	GLuint uniform_textureOffset;
};

