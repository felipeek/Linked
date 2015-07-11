#pragma once
#include <GL/glew.h>
#include <string>
#include "Transform.h"

class Shader
{
public:
	Shader(std::string&);
	~Shader();
	GLuint getShader();
	GLuint getActiveShader();
	void useShader();
	void stopShader();
	virtual void update(Transform* transform);

private:
	GLuint loadShader(const char*, GLuint);
	virtual void getUniformLocations();
protected:
	GLuint shader;
};

