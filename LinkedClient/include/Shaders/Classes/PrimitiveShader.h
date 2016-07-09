#pragma once
#include "Shader.h"

class Light;
class Player;

class PrimitiveShader : public Shader
{
public:
	PrimitiveShader(std::string fileName, Camera* camera, Light* light);
	~PrimitiveShader();
	virtual void update();

	void setPlayer(Player* player) { this->player = player; }
private:
	virtual void getUniformLocations();
	GLuint uniform_TexSampler;
	GLuint uniform_Model;
	GLuint uniform_viewProj;
	GLuint uniform_textureNumRows;
	GLuint uniform_textureOffset;
	GLuint uniform_lightPosition;
	GLuint uniform_lightColor;
	GLuint uniform_secondLightPos;

	Light* light;
	Player* player;
};

