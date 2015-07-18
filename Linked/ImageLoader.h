#pragma once
#include <string>
#include <glm/glm.hpp>
#include <GL\glew.h>

using namespace glm;

class PixelOutOfBoundsException : std::exception{};

class ImageLoader
{
public:
	ImageLoader(std::string& filename, int nChannels);
	ImageLoader();
	~ImageLoader();
	unsigned char* getLoadedImage();
	virtual int getWidth();
	virtual int getHeight();
	virtual int getChannels();
protected:
	unsigned char* loadedImage;
	int width;
	int height;
	int channels;
	vec3 getPixel(int x, int y);
};
