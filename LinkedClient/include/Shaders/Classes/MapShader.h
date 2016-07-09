#pragma once
#include "Shader.h"
class Light;
class Camera;
class Player;

class MapShader : public Shader
{
public:
	MapShader(std::string fileName, Camera* camera, Light* light);
	~MapShader();
	virtual void update();

	/* GETTERS AND SETTERS */
	void setLight(Light* light);
	void setLightCamera(Camera* c);

	void setPlayer(Player* player) { this->player = player; }
private:
	virtual void getUniformLocations();

	GLuint uniform_NormalFloor;
	GLuint uniform_Blocked;
	GLuint uniform_Water;
	GLuint uniform_Dirt;
	GLuint uniform_BlendMap;
	GLuint uniform_LightPos;
	GLuint uniform_LightIntensity;
	GLuint uniform_lightSpace;
	GLuint uniform_shadowMap;
	GLuint uniform_SecondLightPos;

	GLuint uniform_Model;
	GLuint uniform_viewProj;

	Light* light;
	Camera* lightCamera;
	Player* player;
};