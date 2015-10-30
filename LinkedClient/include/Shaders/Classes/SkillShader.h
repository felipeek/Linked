#pragma once
#include "Shader.h"

class Light;
class Entity;

class SkillShader : public Shader
{
public:
	SkillShader(std::string fileName);
	~SkillShader();
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

