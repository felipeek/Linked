#pragma once
#include <GL\glew.h>
#include <string>
#include "ImageLoader.h"

class Texture : public ImageLoader
{
public:
	Texture(std::string fileName);
	Texture(std::string fileName, float bias);
	Texture();
	~Texture();

	GLuint textureID;
	GLuint genGLTexture();

	std::string getFilename();

	// Implement if necessary
	//virtual unsigned char* getLoadedImage();
protected:
	std::string fileName;
	float bias;
};

class DynamicTexture : public Texture
{
public:
	DynamicTexture(int width, int height, bool mipmap, float bias);
	~DynamicTexture();

	GLuint genDynamicGLTexture();

	// Implement if necessary
	//virtual unsigned char* getLoadedImage();
private:
	
};