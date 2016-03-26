#include "SkillShader.h"

#include "ContextWindow.h"
#include "Entity.h"
#include "Primitive.h"

SkillShader::SkillShader(std::string fileName) : Shader(fileName, NULL)
{
	getUniformLocations();
}


SkillShader::~SkillShader()
{
}

void SkillShader::getUniformLocations()
{
	uniform_TexSampler = glGetUniformLocation(shader, "TextureSampler");
	uniform_Model = glGetUniformLocation(shader, "Model");
	uniform_textureNumRows = glGetUniformLocation(shader, "textureNumRows");
	uniform_textureOffset = glGetUniformLocation(shader, "textureOffset");
}

void SkillShader::update()
{
	int ww = ContextWindow::getCurrent().getWidth();
	int wh = ContextWindow::getCurrent().getHeight();
	glm::mat4 ortho = glm::ortho(-(float)ww/wh, (float)ww/wh, -1.0f, 1.0f);
	glm::mat4 finalMatrix = ortho * entity->getTransform()->model;
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(uniform_TexSampler, 0);
	glUniformMatrix4fv(uniform_Model, 1, GL_FALSE, &finalMatrix[0][0]);

	glUniform2fv(uniform_textureOffset, 1, &entity->getMesh()->getQuad()->getTextureOffset()[0]);
	glUniform1f(uniform_textureNumRows, entity->getMesh()->getQuad()->getTextureNumRows());
}

void SkillShader::useShader()
{
	Shader::useShader();
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SkillShader::stopShader()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	Shader::stopShader();
}