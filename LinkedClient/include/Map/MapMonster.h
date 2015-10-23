#pragma once
#include "ImageLoader.h"
#include "MonsterFactory.h"

class MapMonster
{
public:
	MapMonster();
	~MapMonster();
	Monster *monster;
	bool monsterExists;
	static MapMonster initWithNoMonster();
};

class MapMonsterImageLoader : public ImageLoader
{
public:
	MapMonsterImageLoader(std::string& filename, int nChannels, MonsterFactory* monsterFactory);
	~MapMonsterImageLoader();
	MapMonster getMonster(vec3 coordinateVector);
	bool coordinateHasCollision(vec3 coordinate);
private:
	MonsterFactory *monsterFactory;
};