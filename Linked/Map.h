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
	MapTerrain getMapTerrainWithMovementCollisionForCoordinate(glm::vec3 coordinate);
	MapCoordinate getMapCoordinateForCoordinate(glm::vec3 coordinate);
	MapTerrain getMapTerrainForCoordinate(glm::vec3 coordinate);
private:
	MapTerrainImageLoader* mapTerrainLoader;
	MapGameEntityImageLoader* mapGameEntityLoader;
	MapMonsterImageLoader* mapMonsterLoader;
};