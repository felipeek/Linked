#pragma once
#include <iostream>
#include "MapTerrainImageLoader.h"
#include "MapEntityImageLoader.h"

class Map
{
public:
	Map(std::string& mapTerrainFilename, std::string& mapEntityFileName, int nChannels);
	~Map();
	MapCoordinate getMapCoordinateForPlayerMovement(glm::vec3 coordinate);
	MapCoordinate getMapCoordinateForMapCreation(glm::vec3 coordinate);
private:
	MapTerrainImageLoader* mapTerrainLoader;
	MapEntityImageLoader* mapEntityLoader;
};