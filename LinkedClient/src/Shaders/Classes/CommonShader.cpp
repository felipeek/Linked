#include "CommonShader.h"
#include "Entity.h"
#include "Camera.h"
#include "Light.h"

CommonShader::CommonShader(std::string fileName, Camera* camera, Light* light) : Shader(fileName, camera)
{
	this->light = light;
	getUniformLocations();
}


CommonShader::~CommonShader()
{
}

void CommonShader::getUniformLocations()
{
	uniform_TexSampler = glGetUniformLocation(shader, "TextureSampler");
	uniform_Model = glGetUniformLocation(shader, "Model");
	uniform_viewProj = glGetUniformLocation(shader, "viewProj");
	uniform_lightPosition = glGetUniformLocation(shader, "lightPosition");
	uniform_lightColor = glGetUniformLocation(shader, "lightColor");
	uniform_tileAmt = glGetUniformLocation(shader, "tileAmt");
}

void CommonShader::update()
{
	glUniform1i(uniform_TexSampler, 0);
	glUniformMatrix4fv(uniform_Model, 1, GL_FALSE, &entity->getTransform()->model[0][0]);
	glUniformMatrix4fv(uniform_viewProj, 1, GL_FALSE, &camera->viewProj[0][0]);
	glUniform3fv(uniform_lightPosition, 1, &this->light->lightPosition[0]);
	glUniform3fv(uniform_lightColor, 1, &this->light->lightColor[0]);
	glUniform1f(uniform_tileAmt, entity->getTexture()->getTileAmount());
}
