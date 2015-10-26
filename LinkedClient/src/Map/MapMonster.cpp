#include "MapMonster.h"
#include <iostream>

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

MapMonster MapMonsterImageLoader::getMonster(vec3 coordinateVector)
{
	MapMonster mapMonster;

	try
	{
		vec3 rgb = getPixel((int)coordinateVector.x, (int)coordinateVector.y);
		
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

bool MapMonsterImageLoader::coordinateHasCollision(vec3 coordinate)
{
	return false;
}