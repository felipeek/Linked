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
	~ImageLoader();
protected:
	unsigned char* loadedImage;
	int width;
	int height;
	int channels;
	vec3 getPixel(int x, int y);
};
