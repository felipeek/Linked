#pragma once
#include "ImageLoader.h"

class MonsterFactory;
class Monster;

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
	virtual ~MapMonsterImageLoader();
	MapMonster getMonster(glm::vec3 coordinateVector);
	bool coordinateHasCollision(glm::vec3 coordinate);
private:
	MonsterFactory *monsterFactory;
};