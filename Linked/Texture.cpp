#include "Texture.h"
#include "stb_image.h"

Texture::Texture(std::string fileName) : ImageLoader(fileName, 4)
{
	numRows = 1;
	index = 0;
	calcAtlas();
	textureID = genGLTexture();
}

Texture::Texture(std::string fileName, int numRows, int index) : ImageLoader(fileName, 4)
{
	this->numRows = (float)numRows;
	this->index = (float)index;
	calcAtlas();
	textureID = genGLTexture();
}

Texture::~Texture()
{
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

void Texture::calcAtlas()
{
	int column = (int)index % (int)numRows;
	offset.x = (float)column / (float)numRows;
	int row = (int)index / (int)numRows;
	offset.y = (float)row / (float)numRows;
}

void Texture::setIndex(int i)
{
	index = (float)i;
	calcAtlas();
}