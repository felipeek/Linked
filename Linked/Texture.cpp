#include "Texture.h"
#include "stb_image.h"

Texture::Texture(std::string fileName) : ImageLoader(fileName, 4)
{
	this->fileName = fileName;
	textureID = genGLTexture();
}

Texture::Texture(){}
Texture::~Texture(){}

std::string Texture::getFilename()
{
	return this->fileName;
}

GLuint Texture::genGLTexture()
{
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, loadedImage);

	stbi_image_free(loadedImage);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;
}

DynamicTexture::DynamicTexture(int width, int height, bool mipmap)
{
	this->fileName = "";
	this->width = width;
	this->height = height;
	this->channels = 4;
	//textureID = genDynamicGLTexture(mipmap);
}

DynamicTexture::~DynamicTexture()
{
	//stbi_image_free(loadedImage);
}

GLuint DynamicTexture::genDynamicGLTexture(int bias)
{
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, loadedImage);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, (GLfloat)bias);
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;
}