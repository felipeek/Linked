#pragma once
#include "Shader.h"

class Light;
class Entity;

class PrimitiveShader : public Shader
{
public:
	PrimitiveShader(std::string fileName, Camera* camera, Light* light);
	~PrimitiveShader();
	virtual void update(Transform* transform, Entity* entity);
private:
	virtual void getUniformLocations();
	GLuint uniform_TexSampler;
	GLuint uniform_Model;
	GLuint uniform_viewProj;
	GLuint uniform_textureNumRows;
	GLuint uniform_textureOffset;
	GLuint uniform_lightPosition;
	GLuint uniform_lightColor;

	Light* light;
};

