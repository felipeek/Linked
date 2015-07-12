#pragma once
#include "Shader.h"

class PrimitiveShader : public Shader
{
public:
	PrimitiveShader(std::string fileName, Camera* camera);
	~PrimitiveShader();
	virtual void update(Transform* transform);
private:
	virtual void getUniformLocations();
	GLuint uniform_TexSampler;
	GLuint uniform_Model;
	GLuint uniform_viewProj;
};

