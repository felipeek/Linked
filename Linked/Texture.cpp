#include "Texture.h"
#include "stb_image.h"

Texture::Texture(std::string fileName)
{
	textureID = loadSTB(fileName);
}


Texture::~Texture()
{
	if (loadedImage)
		stbi_image_free(loadedImage);
}

GLuint Texture::loadSTB(std::string& fileName)
{
	//int width, height, channels;
	unsigned char *image = stbi_load(fileName.c_str(), &width, &height, &channels, 4);

	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	stbi_image_free(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;
}