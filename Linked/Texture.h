#pragma once
#include <GL\glew.h>
#include <string>
#include "ImageLoader.h"

class Texture : public ImageLoader
{
public:
	Texture(std::string fileName);
	Texture(std::string fileName, int numRows, int index);
	~Texture();
	GLuint textureID;
	GLuint genGLTexture();
	void setIndex(int i);
	float numRows;
	glm::vec2 offset;
	std::string getFilename();
private:
	float index;
	void calcAtlas();
	std::string fileName;
};

