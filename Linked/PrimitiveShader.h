#pragma once
#include "Shader.h"
class Texture;
class Light;

class PrimitiveShader : public Shader
{
public:
	PrimitiveShader(std::string fileName, Camera* camera);
	~PrimitiveShader();
	virtual void update(Transform* transform, Texture* texture, Light* light);
private:
	virtual void getUniformLocations();
	GLuint uniform_TexSampler;
	GLuint uniform_Model;
	GLuint uniform_viewProj;
	GLuint uniform_textureNumRows;
	GLuint uniform_textureOffset;
	GLuint uniform_lightPosition;
	GLuint uniform_lightColor;
};

