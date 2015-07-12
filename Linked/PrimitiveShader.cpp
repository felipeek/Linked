#include "PrimitiveShader.h"


PrimitiveShader::PrimitiveShader(std::string fileName, Camera* camera) : Shader(fileName, camera)
{
	getUniformLocations();
}


PrimitiveShader::~PrimitiveShader()
{
}

void PrimitiveShader::getUniformLocations()
{
	uniform_TexSampler = glGetUniformLocation(shader, "TextureSampler");
	uniform_Model = glGetUniformLocation(shader, "Model");
	uniform_viewProj = glGetUniformLocation(shader, "viewProj");
}

void PrimitiveShader::update(Transform* transform)
{
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(uniform_TexSampler, 0);
	glUniformMatrix4fv(uniform_Model, 1, GL_FALSE, &transform->model[0][0]);
	glUniformMatrix4fv(uniform_viewProj, 1, GL_FALSE, &camera->viewProj[0][0]);
}