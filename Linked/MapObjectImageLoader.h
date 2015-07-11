#pragma once
#include "ImageLoader.h"
#include <iostream>

class MapObjectImageLoader : public ImageLoader
{
public:
	MapObjectImageLoader(std::string& filename, int nChannels);
	~MapObjectImageLoader();
	MapObject getMapObject(vec3 coordinate);
private:
	MapObject transformRgbIntoMapObject(vec3 rgb);
};