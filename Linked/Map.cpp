#include "Map.h"


Map::Map(std::string& mapTerrainFilename, std::string& mapEntityFileName, int nChannels)
{
	mapTerrainLoader = new MapTerrainImageLoader(mapTerrainFilename, nChannels);
	mapEntityLoader = new MapEntityImageLoader(mapEntityFileName, nChannels);
}


Map::~Map()
{
	delete mapTerrainLoader;
	delete mapEntityLoader;
}

MapCoordinate Map::getMapCoordinate(glm::vec3 coordinate)
{
	MapCoordinate mapCoordinates;

	mapCoordinates.terrain = mapTerrainLoader->getMapTerrain(coordinate);
	mapCoordinates.entity = mapEntityLoader->getMapEntity(coordinate);

	return mapCoordinates;
}