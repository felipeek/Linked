#include "MapMonster.h"

MapMonsterImageLoader::MapMonsterImageLoader(std::string& filename, int nChannels, MonsterFactory* monsterFactory) : ImageLoader(filename, nChannels)
{
	this->monsterFactory = monsterFactory;
}

MapMonsterImageLoader::~MapMonsterImageLoader()
{
}

MapMonster::MapMonster() : monster(NULL, NULL)
{
}

MapMonster::~MapMonster()
{
}

MapMonster MapMonsterImageLoader::getMonster(vec3 coordinateVector)
{
	MapMonster mapMonster = MapMonster();

	try
	{
		vec3 rgb = getPixel((int)coordinateVector.x, (int)coordinateVector.y);
		mapMonster.monster = monsterFactory->getMonsterOfMapColor(rgb);
		mapMonster.monsterExists = true;
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