#pragma once
#include <GL/glew.h>
#include <string>
#include "Transform.h"
#include "Camera.h"
#include "Texture.h"

class Shader
{
public:
	Shader(std::string, Camera* camera);
	~Shader();
	GLuint getShader();
	GLuint getActiveShader();
	void useShader();
	void stopShader();
	virtual void update(Transform* transform, Texture* texture);

private:
	GLuint loadShader(const char*, GLuint);
	virtual void getUniformLocations();
protected:
	GLuint shader;
	Camera* camera;
};

