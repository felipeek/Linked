#include "MonsterFactory.h"
#include "rapidxml_utils.hpp"

using namespace rapidxml;
using namespace std;

MonsterFactory::MonsterFactory()
{
	parseAllMonstersInDirectory();
}


MonsterFactory::~MonsterFactory()
{
}

std::vector<Monster*> MonsterFactory::getListOfAllMonsters()
{
	return monsters;
}

Monster* MonsterFactory::getMonsterOfName(std::string name)
{
	for (Monster* monster : monsters)
		if (monster->getName() == name)
			return generateCopyOfMonster(monster);

	throw MonsterNotFoundException();
}

Monster* MonsterFactory::getMonsterOfMapColor(glm::vec3 color)
{
	for (Monster* monster : monsters)
		if (monster->getMapColorRed() == color.r)
			if (monster->getMapColorGreen() == color.g)
				if (monster->getMapColorBlue() == color.b)
					return generateCopyOfMonster(monster);

	throw MonsterNotFoundException();
}

bool MonsterFactory::isMonsterMapColorValid(glm::vec3 color)
{
	for (glm::vec3 validColor : validColors)
		if (color.r == validColor.r)
			if (color.g == validColor.g)
				if (color.b == validColor.b)
					return true;

	return false;
}

void MonsterFactory::parseAllMonstersInDirectory()
{
	for (std::string str : getListOfFilesInDirectory(MONSTERS_DIRECTORY))
	{
		std::string fullPath = MONSTERS_DIRECTORY + str;
		std::vector<char> writable(fullPath.begin(), fullPath.end());
		writable.push_back('\0');
		Monster* monster = parseXmlMonster(&writable[0]);
		monsters.push_back(monster);
	}
}

Monster* MonsterFactory::generateCopyOfMonster(Monster* monster)
{
	Monster* copy = new Monster();

	// Copy Basic Attributes
	copy->setTotalAttack(monster->getTotalAttack());
	copy->setTotalDefense(monster->getTotalDefense());
	copy->setHp(monster->getHp());
	copy->setTotalMaximumHp(monster->getTotalMaximumHp());
	copy->setMapColor(monster->getMapColor());
	copy->setName(monster->getName());
	copy->setTotalSpeed(monster->getTotalSpeed());
	copy->setTotalRange(monster->getTotalRange());
	copy->setTotalCollisionRange(monster->getTotalCollisionRange());
	copy->setTotalAttackSpeed(monster->getTotalAttackSpeed());

	copy->setPosition(monster->getPosition());

	return copy;
}

Monster* MonsterFactory::parseXmlMonster(char* monsterPath)
{
	file<> xmlFile(monsterPath);
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	Monster* monster = new Monster();
	std::string xmlRootNodeName = std::string(doc.first_node()->name());

	monster->setPosition(MONSTERS_STANDARD_POSITION);

	if (xmlRootNodeName == MONSTERS_ROOT_NODE)
	{
		xml_node<> *rootNode = doc.first_node();

		for (xml_node<> *child = rootNode->first_node(); child; child = child->next_sibling())
		{
			std::string nodeName = std::string(child->name());
			char* nodeValue = child->value();

			if (nodeName == MONSTERS_NAME_NODE)
				monster->setName(std::string(nodeValue));
			else if (nodeName == MONSTERS_COLLISIONRANGE_NODE)
				monster->setTotalCollisionRange(std::atoi(nodeValue));
			else if (nodeName == MONSTERS_MAXHP_NODE)
				monster->setTotalMaximumHp(std::atoi(nodeValue));
			else if (nodeName == MONSTERS_HP_NODE)
				monster->setHp(std::atoi(nodeValue));
			else if (nodeName == MONSTERS_ATTACK_NODE)
				monster->setTotalAttack(std::atoi(nodeValue));
			else if (nodeName == MONSTERS_DEFENSE_NODE)
				monster->setTotalDefense(std::atoi(nodeValue));
			else if (nodeName == MONSTERS_SPEED_NODE)
				monster->setTotalSpeed(std::atoi(nodeValue));
			else if (nodeName == MONSTERS_RANGE_NODE)
				monster->setTotalRange(std::atoi(nodeValue));
			else if (nodeName == MONSTERS_ATTACKSPEED_NODE)
				monster->setTotalAttackSpeed(std::atoi(nodeValue));
			else if (nodeName == MONSTERS_RED_NODE)
				monster->setMapColorRed(std::atoi(nodeValue));
			else if (nodeName == MONSTERS_GREEN_NODE)
				monster->setMapColorGreen(std::atoi(nodeValue));
			else if (nodeName == MONSTERS_BLUE_NODE)
				monster->setMapColorBlue(std::atoi(nodeValue));
		}
	}

	if (!isMonsterMapColorValid(monster->getMapColor()))
		validColors.push_back(monster->getMapColor());

	return monster;
}