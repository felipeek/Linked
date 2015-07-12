#pragma once
#include "ImageLoader.h"

class MapTerrainImageLoader : public ImageLoader
{
public:
	MapTerrainImageLoader(std::string& filename, int nChannels);
	~MapTerrainImageLoader();
	MapTerrain getMapTerrain(vec3 coordinate);
private:
	MapTerrain transformRgbIntoMapTerrain(vec3 rgb);
};