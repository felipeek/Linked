#include "MapShader.h"
#include "Light.h"
#include "Transform.h"
#include "Camera.h"

MapShader::MapShader(std::string fileName, Camera* camera) : Shader(fileName, camera)
{
	getUniformLocations();
}


MapShader::~MapShader()
{
}
#include "PrimitiveShader.h"


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
}

void MapShader::update(Transform* transform, Light* light)
{
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(uniform_NormalFloor, 0);
	glActiveTexture(GL_TEXTURE1);
	glUniform1i(uniform_Blocked, 1);
	glActiveTexture(GL_TEXTURE2);
	glUniform1i(uniform_Water, 2);
	glActiveTexture(GL_TEXTURE3);
	glUniform1i(uniform_Dirt, 3);
	glActiveTexture(GL_TEXTURE3);
	glUniform1i(uniform_BlendMap, 4);

	glUniform3fv(uniform_LightPos, 1, &light->lightPosition[0]);
	glUniform3fv(uniform_LightIntensity, 1, &light->lightColor[0]);

	glUniformMatrix4fv(uniform_Model, 1, GL_FALSE, &transform->model[0][0]);
	glUniformMatrix4fv(uniform_viewProj, 1, GL_FALSE, &camera->viewProj[0][0]);
}