#include "Texture.h"
#include "stb_image.h"

Texture::Texture(std::string fileName) : Texture(fileName, 0){}

Texture::Texture(std::string fileName, float bias) : ImageLoader(fileName, 4)
{
	this->bias = bias;
	this->fileName = fileName;
	this->tileAmount = 1.0f;
	this->interpolationMethod = TexInterp::NEAREST_MIPMAP;
	textureID = genGLTexture();
}

Texture::Texture(int width, int height)
{
	textureID = genGLNullTexture(width, height);
}

Texture::Texture(){}
Texture::~Texture(){}

std::string Texture::getFilename()
{
	return this->fileName;
}

float Texture::getTileAmount()
{
	return this->tileAmount;
}

void Texture::setTileAmount(float amt)
{
	this->tileAmount = amt;
}

GLuint Texture::genGLTexture()
{
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, loadedImage);

	stbi_image_free(loadedImage);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	setInterpolationMethod(interpolationMethod);

	return textureID;
}

GLuint Texture::genGLNullTexture(int width, int height)
{
	// Generate texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}

void Texture::setInterpolationMethod(TexInterp method)
{
	this->interpolationMethod = method;
	if (interpolationMethod == TexInterp::LINEAR_MIPMAP)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else if (interpolationMethod == TexInterp::LINEAR)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	else if (interpolationMethod == TexInterp::NEAREST_MIPMAP)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	else if (interpolationMethod == TexInterp::NEAREST)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	if (interpolationMethod == TexInterp::NEAREST_MIPMAP || interpolationMethod == TexInterp::LINEAR_MIPMAP)
	{
		if (bias < 0)
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, (GLfloat)bias);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

DynamicTexture::DynamicTexture(int width, int height, bool mipmap, float bias)
{
	this->fileName = "";
	this->width = width;
	this->height = height;
	this->channels = 4;
	this->bias = bias;
	//textureID = genDynamicGLTexture(mipmap);
}

DynamicTexture::~DynamicTexture()
{
	//stbi_image_free(loadedImage);
}

GLuint DynamicTexture::genDynamicGLTexture()
{
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, loadedImage);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return textureID;
}

int& Texture::getReferenceCount()
{
	return this->referenceCount;
}
void Texture::setReferenceCount(int refCount)
{
	this->referenceCount = refCount;
}