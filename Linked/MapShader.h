#pragma once
#include "Shader.h"
class Light;

class MapShader : public Shader
{
public:
	MapShader(std::string fileName, Camera* camera, Light* light);
	~MapShader();
	virtual void update(Transform* transform);
private:
	virtual void getUniformLocations();

	GLuint uniform_NormalFloor;
	GLuint uniform_Blocked;
	GLuint uniform_Water;
	GLuint uniform_Dirt;
	GLuint uniform_BlendMap;
	GLuint uniform_LightPos;
	GLuint uniform_LightIntensity;

	GLuint uniform_Model;
	GLuint uniform_viewProj;

	Light* light;
};