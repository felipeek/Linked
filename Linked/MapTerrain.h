#pragma once
#include "ImageLoader.h"

enum MapTerrain {
	NORMAL_FLOOR,
	BLOCKED,
	WATER,
	DIRT
};

class MapTerrainImageLoader : public ImageLoader
{
public:
	MapTerrainImageLoader(std::string& filename, int nChannels);
	~MapTerrainImageLoader();
	MapTerrain getMapTerrainForPlayerMovement(glm::vec3 coordinate);
	MapTerrain getMapTerrainForMapCreation(glm::vec3 coordinateVector);
	static bool isOfCollisionType(MapTerrain terrain);
private:
	MapTerrain transformRgbIntoMapTerrain(glm::vec3 rgb);
};