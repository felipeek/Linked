#include "Map.h"


Map::Map(std::string& mapObjectsFilename, std::string& mapHeightsFilename, int nChannels)
{
	mapObjectLoader = new MapObjectImageLoader(mapObjectsFilename, nChannels);
	mapHeightLoader = new MapHeightImageLoader(mapHeightsFilename, nChannels);
}


Map::~Map()
{
	delete mapObjectLoader;
	delete mapHeightLoader;
}

MapCoordinate Map::getMapCoordinate(glm::vec3 coordinate)
{
	MapCoordinate mapCoordinates;

	mapCoordinates.object = mapObjectLoader->getMapObject(coordinate);
	mapCoordinates.height = mapHeightLoader->getMapHeight(coordinate);

	return mapCoordinates;
}