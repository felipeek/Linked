#include "MapEntity.h"

using namespace glm;

MapGameEntity::MapGameEntity()
{
	gameEntity = NULL;
}

MapGameEntity::~MapGameEntity()
{

}

MapGameEntity MapGameEntity::initWithNoGameEntity()
{
	MapGameEntity mapGameEntity;
	mapGameEntity.gameEntityExists = false;
	return mapGameEntity;
}

MapGameEntityImageLoader::MapGameEntityImageLoader(std::string& filename, int nChannels, GameEntityFactory* gameEntityFactory) : ImageLoader(filename, nChannels)
{
	this->gameEntityFactory = gameEntityFactory;
}


MapGameEntityImageLoader::~MapGameEntityImageLoader()
{
}

MapGameEntity MapGameEntityImageLoader::getMapEntity(vec3 coordinateVector)
{
	MapGameEntity mapGameEntity = MapGameEntity();

	try
	{
		vec3 rgb = getPixel((int)coordinateVector.x, (int)coordinateVector.y);

		if (gameEntityFactory->isGameEntityMapColorValid(rgb))
		{
			mapGameEntity.gameEntity = gameEntityFactory->getGameEntityOfMapColor(rgb);
			mapGameEntity.gameEntityExists = true;
		}
		else
			mapGameEntity.gameEntityExists = false;
	}
	catch (PixelOutOfBoundsException e)
	{
		mapGameEntity.gameEntityExists = false;
	}
	catch (GameEntityNotFoundException e)
	{
		mapGameEntity.gameEntityExists = false;
	}

	return mapGameEntity;
}

bool MapGameEntityImageLoader::coordinateHasCollision(vec3 coordinate)
{
	vec3 rgb = getPixel((int)coordinate.x, (int)coordinate.y);

	if (rgb.r == DEFAULT_ENTITIES_MAP_COLOR_RED && rgb.g == DEFAULT_ENTITIES_MAP_COLOR_GREEN && rgb.b == DEFAULT_ENTITIES_MAP_COLOR_BLUE)
		return false;

	return true;
}