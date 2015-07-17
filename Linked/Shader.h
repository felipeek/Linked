#pragma once
#include <GL/glew.h>
#include <string>
class Transform;
class Camera;
class Texture;
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
	virtual void update(Transform* transform, Texture* texture, Light* light);

private:
	GLuint loadShader(const char*, GLuint);
	virtual void getUniformLocations();
protected:
	GLuint shader;
	Camera* camera;
};

