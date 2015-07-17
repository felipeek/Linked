#include "PrimitiveShader.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"

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
	uniform_textureNumRows = glGetUniformLocation(shader, "textureNumRows");
	uniform_textureOffset = glGetUniformLocation(shader, "textureOffset");
	uniform_lightPosition = glGetUniformLocation(shader, "lightPosition");
	uniform_lightColor = glGetUniformLocation(shader, "lightColor");
}

void PrimitiveShader::update(Transform* transform, Texture* texture, Light* light)
{
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(uniform_TexSampler, 0);
	glUniformMatrix4fv(uniform_Model, 1, GL_FALSE, &transform->model[0][0]);
	glUniformMatrix4fv(uniform_viewProj, 1, GL_FALSE, &camera->viewProj[0][0]);

	glUniform1f(uniform_textureNumRows, texture->numRows);
	glUniform2fv(uniform_textureOffset, 1, &texture->offset[0]);

	glUniform3fv(uniform_lightPosition, 1, &light->lightPosition[0]);
	glUniform3fv(uniform_lightColor, 1, &light->lightColor[0]);
}