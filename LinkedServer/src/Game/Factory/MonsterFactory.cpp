#include "MonsterFactory.h"
#include "BasicMonster.h"
#include "TurretMonster.h"

MonsterFactory::MonsterFactory()
{
	parseAllMonstersInDirectory();
}


MonsterFactory::~MonsterFactory()
{
	for (int i = 0, size = monsters.size(); i < size; i++)
	{
		delete monsters[0];
		monsters.erase(monsters.begin());
	}
}

std::vector<Monster*> MonsterFactory::getListOfAllMonsters()
{
	return monsters;
}

Monster* MonsterFactory::getMonsterOfName(std::string name)
{
	for (Monster* monster : monsters)
		if (monster->getName() == name)
			return monster->getCopy(nullptr);

	throw MonsterNotFoundException();
}

Monster* MonsterFactory::getMonsterOfMapColor(glm::vec3 color)
{
	for (Monster* monster : monsters)
		if (monster->getMapColorRed() == color.r)
			if (monster->getMapColorGreen() == color.g)
				if (monster->getMapColorBlue() == color.b)
					return monster->getCopy(nullptr);

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

Monster* MonsterFactory::parseXmlMonster(char* monsterPath)
{
	rapidxml::file<> xmlFile(monsterPath);
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	std::string xmlRootNodeName = std::string(doc.first_node()->name());
	rapidxml::xml_node<> *rootNode = doc.first_node();

	Monster* parsedMonster = NULL;

	if (xmlRootNodeName == MONSTERS_ROOT_NODE)
	{
		MonsterType type = this->findMonsterType(rootNode->first_node());

		if (type == MonsterType::BASIC)
		{
			BasicMonster* basicMonster = new BasicMonster();
			this->fillBasicMonsterAttributes(basicMonster, rootNode->first_node());
			this->fillGenericMonsterAttributes(basicMonster, rootNode->first_node());
			parsedMonster = basicMonster;
		}
		else if (type == MonsterType::GENERIC)
		{
			Monster* genericMonster = new Monster();
			this->fillGenericMonsterAttributes(genericMonster, rootNode->first_node());
			parsedMonster = genericMonster;
		}
		else if (type == MonsterType::TURRET)
		{
			TurretMonster* turretMonster = new TurretMonster();
			this->fillTurretMonsterAttributes(turretMonster, rootNode->first_node());
			this->fillGenericMonsterAttributes(turretMonster, rootNode->first_node());
			parsedMonster = turretMonster;
		}
	}

	return parsedMonster;
}

MonsterType MonsterFactory::findMonsterType(rapidxml::xml_node<> *firstNode)
{
	for (rapidxml::xml_node<> *child = firstNode; child; child = child->next_sibling())
	{
		std::string nodeName = std::string(child->name());
		char* nodeValue = child->value();

		if (nodeName == MONSTERS_TYPE_NODE)
			return this->decodeMonsterType(nodeValue);
	}

	return MonsterType::GENERIC;
}

void MonsterFactory::fillGenericMonsterAttributes(Monster* monster, rapidxml::xml_node<> *firstNode)
{
	monster->setPosition(MONSTERS_STANDARD_POSITION);

	for (rapidxml::xml_node<> *child = firstNode; child; child = child->next_sibling())
	{
		std::string nodeName = std::string(child->name());
		char* nodeValue = child->value();

		if (nodeName == MONSTERS_NAME_NODE)
			monster->setName(std::string(nodeValue));
		else if (nodeName == MONSTERS_TEXTUREQUANTITY_NODE)
			monster->setTextureQuantity(std::atoi(nodeValue));
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

	if (!isMonsterMapColorValid(monster->getMapColor()))
		validColors.push_back(monster->getMapColor());
}

void MonsterFactory::fillBasicMonsterAttributes(BasicMonster* monster, rapidxml::xml_node<> *firstNode)
{

}

void MonsterFactory::fillTurretMonsterAttributes(TurretMonster* monster, rapidxml::xml_node<> *firstNode)
{

}

MonsterType MonsterFactory::decodeMonsterType(std::string encodedType)
{
	if (encodedType.compare("BASIC") == 0)
		return MonsterType::BASIC;
	else if (encodedType.compare("GENERIC") == 0)
		return MonsterType::GENERIC;
	else if (encodedType.compare("TURRET") == 0)
		return MonsterType::TURRET;

	return MonsterType::GENERIC;
}