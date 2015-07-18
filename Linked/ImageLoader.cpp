#define STB_IMAGE_IMPLEMENTATION

#include "ImageLoader.h"
#include "include\stb_image.h"
#include <glm\glm.hpp>

using namespace glm;

ImageLoader::ImageLoader(std::string& filename, int nChannels)
{
	loadedImage = stbi_load(filename.c_str(), &width, &height, &channels, nChannels);
}

ImageLoader::ImageLoader()
{
	loadedImage = NULL;
}

ImageLoader::~ImageLoader()
{
	if (loadedImage)
		stbi_image_free(loadedImage);
}

vec3 ImageLoader::getPixel(int x, int y)
{
	if (y < 0 || x < 0 || y > width || x > height)
		throw PixelOutOfBoundsException();

	vec3 rgb_vector = vec3();
	rgb_vector.r = floor((float)loadedImage[(y * width * 3) + x * 3]);
	rgb_vector.g = floor((float)loadedImage[(y * width * 3) + x * 3 + 1]);
	rgb_vector.b = floor((float)loadedImage[(y * width * 3) + x * 3 + 2]);
	/*std::cout << "X: " << x << std::endl;
	std::cout << "Y: " << y << std::endl;
	std::cout << "rgb: (" << rgb_vector.r << ", " << rgb_vector.g << ", " << rgb_vector.b << ")" << std::endl;*/
	return rgb_vector;
}

int ImageLoader::getWidth()
{
	return width;
}
int ImageLoader::getHeight()
{
	return height;
}
int ImageLoader::getChannels()
{
	return channels;
}

unsigned char* ImageLoader::getLoadedImage()
{
	return loadedImage;
}