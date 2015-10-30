#include "MapMonster.h"
#include "MonsterFactory.h"

MapMonster::MapMonster()
{
	monster = NULL;
}

MapMonster::~MapMonster()
{
}

MapMonster MapMonster::initWithNoMonster()
{
	MapMonster mapMonster;
	mapMonster.monsterExists = false;
	return mapMonster;
}

MapMonsterImageLoader::MapMonsterImageLoader(std::string& filename, int nChannels, MonsterFactory* monsterFactory) : ImageLoader(filename, nChannels)
{
	this->monsterFactory = monsterFactory;
}

MapMonsterImageLoader::~MapMonsterImageLoader()
{
	freeLoadedImage();
}

MapMonster MapMonsterImageLoader::getMonster(glm::vec3 coordinateVector)
{
	MapMonster mapMonster;

	try
	{
		glm::vec3 rgb = getPixel((int)coordinateVector.x, (int)coordinateVector.y);
		
		if (rgb.r == 0 && rgb.g == 0 && rgb.b == 0)
		{
			mapMonster.monsterExists = false;
			return mapMonster;
		}
		if (monsterFactory->isMonsterMapColorValid(rgb))
		{
			mapMonster.monster = monsterFactory->getMonsterOfMapColor(rgb);
			mapMonster.monsterExists = true;
		}
		else
			mapMonster.monsterExists = false;

	}
	catch (PixelOutOfBoundsException e)
	{
		mapMonster.monsterExists = false;
	}
	catch (MonsterNotFoundException e)
	{
		mapMonster.monsterExists = false;
	}

	return mapMonster;
}

bool MapMonsterImageLoader::coordinateHasCollision(glm::vec3 coordinate)
{
	return false;
}