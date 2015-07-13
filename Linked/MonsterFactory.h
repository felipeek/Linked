#pragma once
#include <vector>
#include <string>
#include "Monster.h"

class MonsterNotFoundException : std::exception{};

class MonsterFactory
{
public:
	MonsterFactory();
	~MonsterFactory();
	std::vector<Monster> getListOfAllMonsters();
	Monster getMonsterOfName(std::string name);
	Monster getMonsterOfMapColor(glm::vec3 color);
private:
	std::vector<std::string> getListOfFilesInDirectory();
	Monster parseXmlMonster(char* monsterPath);
	void parseAllMonstersInDirectory();
	std::vector<Monster> monsters;
};