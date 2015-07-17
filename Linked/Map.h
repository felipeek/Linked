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
	MapCoordinate getMapCoordinateForPlayerMovement(glm::vec3 coordinate);
	MapCoordinate getMapCoordinateForMapCreation(glm::vec3 coordinate);
private:
	MapTerrainImageLoader* mapTerrainLoader;
	MapGameEntityImageLoader* mapGameEntityLoader;
	MapMonsterImageLoader* mapMonsterLoader;
};