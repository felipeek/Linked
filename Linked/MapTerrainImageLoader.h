#pragma once
#include "ImageLoader.h"

class MapTerrainImageLoader : public ImageLoader
{
public:
	MapTerrainImageLoader(std::string& filename, int nChannels);
	~MapTerrainImageLoader();
	MapTerrain getMapTerrainForPlayerMovement(vec3 coordinate);
	MapTerrain getMapTerrainForMapCreation(vec3 coordinateVector);
private:
	MapTerrain transformRgbIntoMapTerrain(vec3 rgb);
};