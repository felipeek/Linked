#pragma once
#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>

class Shader2
{
public:
	Shader2(std::string);
	~Shader2();
	GLuint getShader();

	void useShader();
	void stopShader();
	void update();

	void activateAlphaBlend();
	void deactivateAlphaBlend();

	void setTextColor(glm::vec4& color);

	GLuint shader;
	GLuint uniformProjection;
	GLuint uniformText;
	GLuint uniformTextColor;
private:
	GLuint loadShader(const char*, GLuint);
	virtual void getUniformLocations();

	glm::vec4 color;
};

