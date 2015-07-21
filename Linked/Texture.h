#pragma once
#include <GL\glew.h>
#include <string>
#include "ImageLoader.h"

class Texture : public ImageLoader
{
public:
	Texture(std::string fileName);
	Texture();
	~Texture();

	GLuint textureID;
	GLuint genGLTexture();

	std::string getFilename();

	// Implement if necessary
	//virtual unsigned char* getLoadedImage();
protected:
	std::string fileName;
};

class DynamicTexture : public Texture
{
public:
	DynamicTexture(int width, int height, bool mipmap);
	~DynamicTexture();

	GLuint genDynamicGLTexture(int bias);

	// Implement if necessary
	//virtual unsigned char* getLoadedImage();
private:
	
};