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
	~MapTerrainImageLoader();
	MapTerrain getMapTerrainWithMovementCollisionForCoordinate(glm::vec3 coordinate);
	MapTerrain getMapTerrainForCoordinate(glm::vec3 coordinateVector);
	static bool isOfCollisionType(MapTerrain terrain);
private:
	MapTerrain transformRgbIntoMapTerrain(glm::vec3 rgb);
};