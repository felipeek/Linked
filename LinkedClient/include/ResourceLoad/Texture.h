#pragma once
#include <GL\glew.h>
#include <string>
#include "ImageLoader.h"

enum class TexInterp
{
	NEAREST_MIPMAP,
	NEAREST,
	LINEAR_MIPMAP,
	LINEAR
};

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
	int& getReferenceCount();
	void setReferenceCount(int refCount);
	void setInterpolationMethod(TexInterp method);
	// Implement if necessary
	//virtual unsigned char* getLoadedImage();
protected:
	std::string fileName;
	float bias;
	float tileAmount;

	int referenceCount;
	TexInterp interpolationMethod;
};

class DynamicTexture : public Texture
{
public:
	DynamicTexture(int width, int height, bool mipmap, float bias);
	virtual ~DynamicTexture();

	GLuint genDynamicGLTexture();

	// Implement if necessary
	//virtual unsigned char* getLoadedImage();
};