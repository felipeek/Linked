#pragma once
#include "ImageLoader.h"
#define THRESHOLD 30

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
	bool coordinateHasCollision(glm::vec3& coordinateVector);
	bool coordinateHasCollisionNoWater(glm::vec3& coordinateVector);
	MapTerrain getMapTerrainForCoordinate(glm::vec3& coordinateVector);
private:
	MapTerrain transformRgbIntoMapTerrain(glm::vec3 rgb);
};