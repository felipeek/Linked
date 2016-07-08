#pragma once
#include "MapTerrain.h"
#include "MapEntity.h"
#include "MapMonster.h"

class MapCoordinate {
public:
	MapCoordinate();
	~MapCoordinate();
	MapTerrain terrain;
	MapGameEntity mapGameEntity;
	MapMonster mapMonster;
};

class Map
{
public:
	Map(std::string& mapTerrainFilename, std::string& mapEntityFilename, int nChannels);
	Map(std::string& mapTerrainFilename, std::string& mapEntityFilename, std::string& mapMonsterFilename, int nChannels, MonsterFactory* monsterFactory, GameEntityFactory* gameEntityFactory);
	~Map();

	// Obs.: This function must be used carefully.
	// This function will eventually create a new monster and/or a new gameEntity on heap and return it inside MapCoordinate.
	// Must be use ONLY for the map creation and NOT for getting general information about map coordinates.
	MapCoordinate getMapCoordinateForMapCreation(glm::vec3 coordinate);

	bool coordinateHasCollision(glm::vec3 coordinate);
	bool coordinateHasCollisionNoWater(glm::vec3 coordinate);
	MapTerrain getMapTerrainForCoordinate(glm::vec3& coordinate);
private:
	MapTerrainImageLoader* mapTerrainLoader;
	MapGameEntityImageLoader* mapGameEntityLoader;
	MapMonsterImageLoader* mapMonsterLoader;
};