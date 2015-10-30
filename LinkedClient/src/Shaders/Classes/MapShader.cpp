#include "MapShader.h"
#include "Light.h"
#include "Camera.h"
#include "Entity.h"

MapShader::MapShader(std::string fileName, Camera* camera, Light* light) : Shader(fileName, camera)
{
	this->light = light;
	getUniformLocations();
}


MapShader::~MapShader()
{
}

void MapShader::getUniformLocations()
{
	uniform_NormalFloor = glGetUniformLocation(shader, "NormalFloor");
	uniform_Blocked = glGetUniformLocation(shader, "Blocked");
	uniform_Water = glGetUniformLocation(shader, "Water");
	uniform_Dirt = glGetUniformLocation(shader, "Dirt");
	uniform_BlendMap = glGetUniformLocation(shader, "BlendMap");
	uniform_LightPos = glGetUniformLocation(shader, "lightPosition");
	uniform_LightIntensity = glGetUniformLocation(shader, "lightIntensity");

	uniform_Model = glGetUniformLocation(shader, "Model");
	uniform_viewProj = glGetUniformLocation(shader, "viewProj");

	// Shadow stuff
	uniform_lightSpace = glGetUniformLocation(shader, "lightSpaceMatrix");
	uniform_shadowMap = glGetUniformLocation(shader, "shadowMap");
}

void MapShader::update()
{

	glUniform1i(uniform_NormalFloor, 0);
	glUniform1i(uniform_Blocked, 1);
	glUniform1i(uniform_Water, 2);
	glUniform1i(uniform_Dirt, 3);
	glUniform1i(uniform_BlendMap, 4);
	glUniform1i(uniform_shadowMap, 5);

	glUniform3fv(uniform_LightPos, 1, &this->light->lightPosition[0]);
	glUniform3fv(uniform_LightIntensity, 1, &this->light->lightColor[0]);

	glUniformMatrix4fv(uniform_Model, 1, GL_FALSE, &entity->getTransform()->model[0][0]);
	glUniformMatrix4fv(uniform_viewProj, 1, GL_FALSE, &camera->viewProj[0][0]);

	glUniformMatrix4fv(uniform_lightSpace, 1, GL_FALSE, &lightCamera->viewProj[0][0]);

}

void MapShader::setLight(Light* light)
{
	this->light = light;
}

void MapShader::setLightCamera(Camera* c)
{
	this->lightCamera = c;
}