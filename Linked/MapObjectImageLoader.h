#pragma once
#include "ImageLoader.h"
#include <iostream>
#include <unordered_map>

class MapObjectImageLoader : public ImageLoader
{
public:
	MapObjectImageLoader(std::string& filename, int nChannels);
	~MapObjectImageLoader();
	MapObject getMapCoordinate(vec3 coordinate);
private:
	MapObject transformRgbIntoMapObject(vec3 rgb);
};