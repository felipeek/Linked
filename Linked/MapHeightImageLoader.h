#pragma once

#include "ImageLoader.h"

class MapHeightImageLoader : public ImageLoader
{
public:
	MapHeightImageLoader(std::string& filename, int nChannels);
	~MapHeightImageLoader();
	MapHeight getMapHeight(vec3 coordinate);
private:
	MapHeight transformRgbIntoMapHeight(vec3 rgb);
};