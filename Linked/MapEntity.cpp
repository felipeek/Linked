#include "MapEntity.h"

using namespace glm;

MapGameEntityImageLoader::MapGameEntityImageLoader(std::string& filename, int nChannels, GameEntityFactory* gameEntityFactory) : ImageLoader(filename, nChannels)
{
	this->gameEntityFactory = gameEntityFactory;
}


MapGameEntityImageLoader::~MapGameEntityImageLoader()
{
}

MapGameEntity::MapGameEntity()
{
	gameEntity = NULL;
}

MapGameEntity::~MapGameEntity()
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

MapGameEntity MapGameEntity::initWithNoGameEntity()
{
	MapGameEntity mapGameEntity;
	mapGameEntity.gameEntityExists = false;
	return mapGameEntity;
}