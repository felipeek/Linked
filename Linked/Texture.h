#pragma once
#include <GL\glew.h>
#include <string>
#include "ImageLoader.h"

class Texture : public ImageLoader
{
public:
	Texture(std::string fileName);
	~Texture();
	GLuint textureID;
	GLuint loadSTB(std::string& fileName);
};

