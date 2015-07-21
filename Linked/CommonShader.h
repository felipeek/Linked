#pragma once
#include "Shader.h"

class Light;
class Entity;

class CommonShader : public Shader
{
public:
	CommonShader(std::string fileName, Camera* camera, Light* light);
	~CommonShader();
	virtual void update(Transform* transform, Entity* entity);
private:
	virtual void getUniformLocations();
	GLuint uniform_TexSampler;
	GLuint uniform_Model;
	GLuint uniform_viewProj;
	GLuint uniform_lightPosition;
	GLuint uniform_lightColor;

	Light* light;
};

