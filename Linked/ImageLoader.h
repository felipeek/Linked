#pragma once
#include <string>
#include <glm/glm.hpp>
#include "MapCoordinate.h"
#include <GL\glew.h>

using namespace glm;

class ImageLoader
{
public:
	ImageLoader(std::string& filename, int nChannels);
	ImageLoader();
	~ImageLoader();
protected:
	unsigned char* loadedImage;
	int width;
	int height;
	int channels;
	vec3 getPixel(int x, int y);
};

