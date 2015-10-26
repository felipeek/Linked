#pragma once
#include "ImageLoader.h"
#define THRESHOLD 30
#define COLLISION_FACTOR 1

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
	virtual ~MapTerrainImageLoader();
	bool MapTerrainImageLoader::coordinateHasCollision(glm::vec3 coordinateVector);
	MapTerrain getMapTerrainForCoordinate(glm::vec3 coordinateVector);
private:
	MapTerrain transformRgbIntoMapTerrain(glm::vec3 rgb);
};