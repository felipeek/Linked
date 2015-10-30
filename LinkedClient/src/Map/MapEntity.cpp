#include "MapEntity.h"
#include "GameEntityFactory.h"

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
	freeLoadedImage();
}

MapGameEntity MapGameEntityImageLoader::getMapEntity(glm::vec3 coordinateVector)
{
	MapGameEntity mapGameEntity = MapGameEntity();

	try
	{
		glm::vec3 rgb = getPixel((int)coordinateVector.x, (int)coordinateVector.y);
		if ((rgb.r == 0 && rgb.g == 0 && rgb.b == 0) || (rgb.r == 255 && rgb.g == 255 && rgb.b == 0))
		{
			mapGameEntity.gameEntityExists = false;
			return mapGameEntity;
		}
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

bool MapGameEntityImageLoader::coordinateHasCollision(glm::vec3 coordinate)
{
	glm::vec3 rgb = getPixel((int)coordinate.x, (int)coordinate.y);

	if (rgb.r == DEFAULT_ENTITIES_MAP_COLOR_RED && rgb.g == DEFAULT_ENTITIES_MAP_COLOR_GREEN && rgb.b == DEFAULT_ENTITIES_MAP_COLOR_BLUE)
		return false;

	return true;
}