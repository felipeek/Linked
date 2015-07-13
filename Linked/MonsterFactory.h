#pragma once
#include <vector>
#include <string>
#include "Monster.h"

#define MONSTERS_DIRECTORY "./res/Monsters/"
#define FILE_EXTENSION ".xml"
#define READ_DIRECTORY_ERROR "Error trying to read monster directory"
#define ROOT_NODE "MONSTER"
#define NAME_NODE "NAME"
#define HP_NODE "HP"
#define ATTACK_NODE "ATTACK"
#define DEFENSE_NODE "DEFENSE"
#define RED_NODE "RED"
#define GREEN_NODE "GREEN"
#define BLUE_NODE "BLUE"

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