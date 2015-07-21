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
	void useShader();
	void stopShader();
	virtual void update(Transform* transform, Entity* entity, Light* light);

private:
	GLuint loadShader(const char*, GLuint);
	virtual void getUniformLocations();
protected:
	GLuint shader;
	Camera* camera;
};

