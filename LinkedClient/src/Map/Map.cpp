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

bool Map::coordinateHasCollision(glm::vec3 coordinate)
{
	if (!mapTerrainLoader->coordinateHasCollision(coordinate))
		if (!mapMonsterLoader->coordinateHasCollision(coordinate))
			if (!mapGameEntityLoader->coordinateHasCollision(coordinate))
				return false;

	return true;
}

// Can only be used to Map Creation
// It will create new monsters/entity whenever called.
MapCoordinate Map::getMapCoordinateForMapCreation(glm::vec3 coordinate)
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

// can be done because 'getMapTerrainForCoordinate' does not create anything on heap
MapTerrain Map::getMapTerrainForCoordinate(glm::vec3& coordinate)
{
	return mapTerrainLoader->getMapTerrainForCoordinate(coordinate);
}