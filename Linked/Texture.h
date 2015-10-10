#pragma once
#include <GL\glew.h>
#include <string>
#include "ImageLoader.h"

class Texture : public ImageLoader
{
public:
	Texture(std::string fileName);
	Texture(std::string fileName, float bias);
	Texture(int width, int height);
	Texture();
	~Texture();

	GLuint textureID;
	GLuint genGLTexture();
	GLuint genGLNullTexture(int width, int height);

	std::string getFilename();

	float getTileAmount();
	void setTileAmount(float amt);

	// Implement if necessary
	//virtual unsigned char* getLoadedImage();
protected:
	std::string fileName;
	float bias;
	float tileAmount;
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