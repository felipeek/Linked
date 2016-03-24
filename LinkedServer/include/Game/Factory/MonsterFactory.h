#pragma once
#include <vector>
#include <string>
#include <rapidxml_utils.hpp>
#include <glm\glm.hpp>
#include "Factory.h"

#define MONSTERS_DIRECTORY "./res/Monsters/"
#define MONSTERS_ROOT_NODE "MONSTER"
#define MONSTERS_TYPE_NODE "TYPE"
#define MONSTERS_NAME_NODE "NAME"
#define MONSTERS_SPRITE_NODE "SPRITE"
#define MONSTERS_TEXTUREQUANTITY_NODE "TEXTUREQUANTITY"
#define MONSTERS_SIZE_NODE "SIZE"
#define MONSTERS_COLLISIONRANGE_NODE "COLLISIONRANGE"
#define MONSTERS_MAXHP_NODE "MAXHP"
#define MONSTERS_HP_NODE "HP"
#define MONSTERS_ATTACK_NODE "ATTACK"
#define MONSTERS_DEFENSE_NODE "DEFENSE"
#define MONSTERS_SPEED_NODE "SPEED"
#define MONSTERS_RANGE_NODE "RANGE"
#define MONSTERS_ATTACKSPEED_NODE "ATTACKSPEED"
#define MONSTERS_RED_NODE "RED"
#define MONSTERS_GREEN_NODE "GREEN"
#define MONSTERS_BLUE_NODE "BLUE"
#define MONSTERS_STANDARD_POSITION glm::vec3(0, 0, 0)
#define MONSTERS_STANDARD_ANGLE 35
#define MONSTERS_STANDARD_AXIS glm::vec3(1, 0, 0)
#define MONSTERS_STANDARD_SCALE glm::vec3(1, 1, 1)

enum class MonsterType
{
	GENERIC,
	BASIC,
	TURRET,
	DISTANCE
};

class Monster;
class BasicMonster;
class TurretMonster;
class DistanceMonster;

class MonsterNotFoundException : std::exception {};

class MonsterFactory : public Factory
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
	Monster* parseXmlMonster(char* monsterPath);
	void fillGenericMonsterAttributes(Monster* monster, rapidxml::xml_node<> *firstNode);
	void fillBasicMonsterAttributes(BasicMonster* monster, rapidxml::xml_node<> *firstNode);
	void fillTurretMonsterAttributes(TurretMonster* monster, rapidxml::xml_node<> *firstNode);
	void fillDistanceMonsterAttributes(DistanceMonster* monster, rapidxml::xml_node<> *firstNode);
	void parseAllMonstersInDirectory();
	MonsterType findMonsterType(rapidxml::xml_node<> *firstNode);
	std::vector<Monster*> monsters;
	std::vector<glm::vec3> validColors;
	MonsterType decodeMonsterType(std::string encodedType);
};