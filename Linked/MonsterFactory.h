#pragma once
#include <vector>
#include <string>
#include "Monster.h"
#include <vector>

#define MONSTERS_DIRECTORY "./res/Monsters/"
#define FILE_EXTENSION ".xml"
#define READ_DIRECTORY_ERROR "Error trying to read monster directory"
#define ROOT_NODE "MONSTER"
#define NAME_NODE "NAME"
#define SPRITE_NODE "SPRITE"
#define SIZE_NODE "SIZE"
#define HP_NODE "HP"
#define ATTACK_NODE "ATTACK"
#define DEFENSE_NODE "DEFENSE"
#define SPEED_NODE "SPEED"
#define RANGE_NODE "RANGE"
#define RED_NODE "RED"
#define GREEN_NODE "GREEN"
#define BLUE_NODE "BLUE"
#define STANDARD_POSITION vec3(0, 0, 1.0f)
#define STANDARD_ANGLE 35
#define STANDARD_AXIS vec3(1, 0, 0)
#define STANDARD_SCALE vec3(1, 1, 1)

class MonsterNotFoundException : std::exception{};

class MonsterFactory
{
public:
	MonsterFactory();
	~MonsterFactory();
	std::vector<Monster*> getListOfAllMonsters();
	// Obs.: This function will generate a new Monster in the heap.
	// Memory must be managed carefully.
	Monster* getMonsterOfName(std::string name);
	// Obs.: This function will generate a new Monster in the heap.
	// Memory must be managed carefully.
	Monster* getMonsterOfMapColor(glm::vec3 color);
	bool isMonsterMapColorValid(glm::vec3 color);
private:
	std::vector<std::string> getListOfFilesInDirectory();
	Monster* parseXmlMonster(char* monsterPath);
	void parseAllMonstersInDirectory();
	std::vector<Monster*> monsters;
	std::vector<glm::vec3> validColors;
	Monster* generateCopyOfMonster(Monster* monster);
};