#pragma once
#include <GL/glew.h>
#include <string>
class Transform;
class Camera;
class Entity;
class Light;

class Shader
{
public:
	Shader(std::string, Camera* camera);
	~Shader();
	GLuint getShader();
	GLuint getActiveShader();
	virtual void useShader();
	virtual void stopShader();
	virtual void update();
	//virtual void update();

	void activateAlphaBlend();
	void deactivateAlphaBlend();

	void setCamera(Camera* c);
	void setEntity(Entity* e);
private:
	GLuint loadShader(const char*, GLuint);
	virtual void getUniformLocations();
protected:
	GLuint shader;
	Camera* camera;
	Entity* entity;
};

