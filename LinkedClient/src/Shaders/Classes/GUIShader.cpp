#include "GUIShader.h"
#include "Entity.h"
#include "Primitive.h"

GUIShader::GUIShader(std::string fileName) : Shader(fileName, NULL)
{
	getUniformLocations();
}


GUIShader::~GUIShader()
{
}

void GUIShader::getUniformLocations()
{
	uniform_TexSampler = glGetUniformLocation(shader, "TextureSampler");
	uniform_Model = glGetUniformLocation(shader, "Model");
	uniform_textureNumRows = glGetUniformLocation(shader, "textureNumRows");
	uniform_textureOffset = glGetUniformLocation(shader, "textureOffset");
}

void GUIShader::update()
{
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(uniform_TexSampler, 0);
	glUniformMatrix4fv(uniform_Model, 1, GL_FALSE, &entity->getTransform()->model[0][0]);

	glUniform2fv(uniform_textureOffset, 1, &entity->getMesh()->getQuad()->getTextureOffset()[0]);
	glUniform1f(uniform_textureNumRows, entity->getMesh()->getQuad()->getTextureNumRows());
}

void GUIShader::useShader()
{
	Shader::useShader();
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GUIShader::stopShader()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	Shader::stopShader();
}