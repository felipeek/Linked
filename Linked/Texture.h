#pragma once
#include <GL\glew.h>
#include <string>
#include "ImageLoader.h"

class Texture : public ImageLoader
{
public:
	Texture(std::string fileName);
	Texture(std::string fileName, int numRows, int index);
	Texture();
	~Texture();
	GLuint textureID;
	GLuint genGLTexture();
	void setIndex(int i);
	float numRows;
	glm::vec2 offset;
	std::string getFilename();
	// Implement if necessary
	//virtual unsigned char* getLoadedImage();
protected:
	float index;
	void calcAtlas();
	std::string fileName;
};

class DynamicTexture : public Texture
{
public:
	DynamicTexture(int width, int height, int rows, int index, bool mipmap);
	~DynamicTexture();
	// Implement if necessary
	//virtual unsigned char* getLoadedImage();
	GLuint genDynamicGLTexture(int bias);
private:
	
};