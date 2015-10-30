#pragma once
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>

class PixelOutOfBoundsException : std::exception{};

class ImageLoader
{
public:
	ImageLoader(std::string& filename, int nChannels);
	ImageLoader();
	virtual ~ImageLoader();
	unsigned char* getLoadedImage();
	void setLoadedImage(unsigned char*);
	virtual int getWidth();
	virtual int getHeight();
	virtual int getChannels();

protected:
	unsigned char* loadedImage;
	int width;
	int height;
	int channels;
	glm::vec3 getPixel(int x, int y);

	void freeLoadedImage();
};
