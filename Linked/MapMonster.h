#pragma once
#include "ImageLoader.h"
#include "MonsterFactory.h"

class MapMonster
{
public:
	MapMonster();
	~MapMonster();
	Monster monster;
	bool monsterExists;
};

class MapMonsterImageLoader : public ImageLoader
{
public:
	MapMonsterImageLoader(std::string& filename, int nChannels, MonsterFactory* monsterFactory);
	~MapMonsterImageLoader();
	MapMonster getMonster(vec3 coordinateVector);
private:
	MonsterFactory *monsterFactory;
};