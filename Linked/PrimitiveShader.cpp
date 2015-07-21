#include "PrimitiveShader.h"
#include "Entity.h"
#include "Primitive.h"
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

void PrimitiveShader::update(Transform* transform, Entity* entity, Light* light)
{
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(uniform_TexSampler, 0);
	glUniformMatrix4fv(uniform_Model, 1, GL_FALSE, &transform->model[0][0]);
	glUniformMatrix4fv(uniform_viewProj, 1, GL_FALSE, &camera->viewProj[0][0]);

	glm::vec2 offset = glm::vec2(0, 0);
	int numRows;
	if (entity->getMesh()->getQuad() == NULL)
	{
		numRows = 1;
		glUniform2fv(uniform_textureOffset, 1, &offset[0]);
	}
	else
	{
		numRows = (int)entity->getMesh()->getQuad()->getTextureNumRows();
		glUniform2fv(uniform_textureOffset, 1, &entity->getMesh()->getQuad()->getTextureOffset()[0]);
	}

	glUniform1f(uniform_textureNumRows, (GLfloat)numRows);

	glUniform3fv(uniform_lightPosition, 1, &light->lightPosition[0]);
	glUniform3fv(uniform_lightColor, 1, &light->lightColor[0]);
}