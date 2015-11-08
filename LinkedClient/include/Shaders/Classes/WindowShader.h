#pragma once
#include "Display.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

class WindowShader : public Shader
{
public:
	WindowShader(std::string fileName);
	~WindowShader();
	virtual void update(glm::vec2& translation);
	void bindTextures(GLuint textureId);
	void unbindTextures();

	glm::vec2 clipTL;
	glm::vec2 clipBR;	
private:
	virtual void getUniformLocations();
	GLuint uniformTexture, uniformUseTexture;

	GLuint uniformProjection, uniformTextColor, uniformModel;
	GLuint uniformClipTopLeft, uniformClipBotRight;

	glm::vec4 m_backgroundColor;
	glm::mat4 m_projection;
	glm::mat4 m_model;

	int useTexture;
public:
	inline glm::mat4& getProjection(){ return this->m_projection; }
	inline glm::mat4& getModel() { return this->m_model; }
	inline glm::vec4& getBackgroundColor() { return this->m_backgroundColor; }
	inline void setTextColor(const glm::vec4& color){ this->m_backgroundColor = color; }
};

