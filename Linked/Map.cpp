#include "Map.h"

Map::Map(std::string& mapTerrainFilename, std::string& mapEntityFilename, int nChannels)
{
	mapTerrainLoader = new MapTerrainImageLoader(mapTerrainFilename, nChannels);
	mapEntityLoader = new MapEntityImageLoader(mapEntityFilename, nChannels);
}

Map::Map(std::string& mapTerrainFilename, std::string& mapEntityFilename, std::string& mapMonsterFilename, int nChannels, MonsterFactory* monsterFactory)
{
	mapTerrainLoader = new MapTerrainImageLoader(mapTerrainFilename, nChannels);
	mapEntityLoader = new MapEntityImageLoader(mapEntityFilename, nChannels);
	mapMonsterLoader = new MapMonsterImageLoader(mapMonsterFilename, nChannels, monsterFactory);
}

Map::~Map()
{
	if (mapTerrainLoader != NULL)
		delete mapTerrainLoader;
	if (mapEntityLoader != NULL)
		delete mapEntityLoader;
	if (mapMonsterLoader != NULL)
		delete mapMonsterLoader;
}

MapCoordinate::MapCoordinate()
{
}

MapCoordinate::~MapCoordinate()
{
}

MapCoordinate Map::getMapCoordinateForPlayerMovement(glm::vec3 coordinate)
{
	MapCoordinate mapCoordinates;

	mapCoordinates.terrain = mapTerrainLoader->getMapTerrainForPlayerMovement(coordinate);
	mapCoordinates.entity = mapEntityLoader->getMapEntity(coordinate);

	if (mapMonsterLoader != NULL)
		mapCoordinates.mapMonster = mapMonsterLoader->getMonster(coordinate);
	else
		mapCoordinates.mapMonster = MapMonster::initWithNoMonster();

	return mapCoordinates;
}

MapCoordinate Map::getMapCoordinateForMapCreation(glm::vec3 coordinate)
{
	MapCoordinate mapCoordinates;

	mapCoordinates.terrain = mapTerrainLoader->getMapTerrainForMapCreation(coordinate);
	mapCoordinates.entity = mapEntityLoader->getMapEntity(coordinate);
	
	if (mapMonsterLoader != NULL)
		mapCoordinates.mapMonster = mapMonsterLoader->getMonster(coordinate);
	else
		mapCoordinates.mapMonster = MapMonster::initWithNoMonster();

	return mapCoordinates;
}