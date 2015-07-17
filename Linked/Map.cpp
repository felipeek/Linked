#include "Map.h"

Map::Map(std::string& mapTerrainFilename, std::string& mapEntityFilename, int nChannels)
{
	mapTerrainLoader = new MapTerrainImageLoader(mapTerrainFilename, nChannels);
}

Map::Map(std::string& mapTerrainFilename, std::string& mapEntityFilename, std::string& mapMonsterFilename, int nChannels, MonsterFactory* monsterFactory, GameEntityFactory* gameEntityFactory)
{
	mapTerrainLoader = new MapTerrainImageLoader(mapTerrainFilename, nChannels);
	mapGameEntityLoader = new MapGameEntityImageLoader(mapEntityFilename, nChannels, gameEntityFactory);
	mapMonsterLoader = new MapMonsterImageLoader(mapMonsterFilename, nChannels, monsterFactory);
}

Map::~Map()
{
	if (mapTerrainLoader != NULL)
		delete mapTerrainLoader;
	if (mapGameEntityLoader != NULL)
		delete mapGameEntityLoader;
	if (mapMonsterLoader != NULL)
		delete mapMonsterLoader;
}

MapCoordinate::MapCoordinate()
{
}

MapCoordinate::~MapCoordinate()
{
}

MapTerrain Map::getMapTerrainWithMovementCollisionForCoordinate(glm::vec3 coordinate)
{
	MapTerrain mapTerrain;

	mapTerrain = mapTerrainLoader->getMapTerrainWithMovementCollisionForCoordinate(coordinate);

	return mapTerrain;
}

MapTerrain Map::getMapTerrainForCoordinate(glm::vec3 coordinate)
{
	MapTerrain mapTerrain;

	mapTerrain = mapTerrainLoader->getMapTerrainForCoordinate(coordinate);

	return mapTerrain;
}

MapCoordinate Map::getMapCoordinateForCoordinate(glm::vec3 coordinate)
{
	MapCoordinate mapCoordinates;

	mapCoordinates.terrain = mapTerrainLoader->getMapTerrainForCoordinate(coordinate);
	
	if (mapMonsterLoader != NULL)
		mapCoordinates.mapMonster = mapMonsterLoader->getMonster(coordinate);
	else
		mapCoordinates.mapMonster = MapMonster::initWithNoMonster();

	if (mapGameEntityLoader != NULL)
		mapCoordinates.mapGameEntity = mapGameEntityLoader->getMapEntity(coordinate);
	else
		mapCoordinates.mapGameEntity = MapGameEntity::initWithNoGameEntity();

	return mapCoordinates;
}