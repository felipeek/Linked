#pragma once
#include "MapTerrain.h"
#include "MapEntity.h"
#include "MapMonster.h"

class MapCoordinate {
public:
	MapCoordinate();
	~MapCoordinate();
	MapTerrain terrain;
	MapEntity entity;
	MapMonster monster;
};

class Map
{
public:
	Map(std::string& mapTerrainFilename, std::string& mapEntityFilename, int nChannels);
	~Map();
	MapCoordinate getMapCoordinateForPlayerMovement(glm::vec3 coordinate);
	MapCoordinate getMapCoordinateForMapCreation(glm::vec3 coordinate);
private:
	MapTerrainImageLoader* mapTerrainLoader;
	MapEntityImageLoader* mapEntityLoader;
	MapMonsterImageLoader* mapMonsterLoader;
};