#include "MonsterFactory.h"
#include "rapidxml_utils.hpp"
#include "Primitive.h"

using namespace rapidxml;
using namespace std;

MonsterFactory::MonsterFactory()
{
	parseAllMonstersInDirectory();
}


MonsterFactory::~MonsterFactory()
{
	for (int i = 0, size=monsters.size(); i < size; i++)
	{
		monsters[0]->getTexture()->getReferenceCount()--;
		if (monsters[0]->getTexture()->getReferenceCount() == 0)
			delete monsters[0]->getTexture();
			
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
	Monster* copy = new Monster(NULL, NULL, NULL);

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
	// Copy Mesh (A new mesh/quad object must be created for each monster)
	Mesh* myMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f, 7, 7));
	copy->setMesh(myMesh);
	// Copy Texture (The same texture will be setted for all monsters of same class)
	Texture* monsterTexture = monster->getTexture();
	monsterTexture->getReferenceCount()++;
	copy->setTexture(monsterTexture);
	// Copy Transform (A new transform object must be created for each monster)
	Transform *monsterTransform = monster->getTransform();
	vec3 monsterTransformPosition = monsterTransform->getPosition();
	vec3 monsterTransformScale = monsterTransform->getScale();
	copy->setTransform(new Transform(monsterTransformPosition, MONSTERS_STANDARD_ANGLE, MONSTERS_STANDARD_AXIS, monsterTransformScale));

	return copy;
}

Monster* MonsterFactory::parseXmlMonster(char* monsterPath)
{
	file<> xmlFile(monsterPath);
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	Monster* monster = new Monster(NULL, NULL, NULL);
	std::string xmlRootNodeName = std::string(doc.first_node()->name());

	if (xmlRootNodeName == MONSTERS_ROOT_NODE)
	{
		xml_node<> *rootNode = doc.first_node();

		monster->setTransform(new Transform(MONSTERS_STANDARD_POSITION, MONSTERS_STANDARD_ANGLE, MONSTERS_STANDARD_AXIS, MONSTERS_STANDARD_SCALE));

		for (xml_node<> *child = rootNode->first_node(); child; child = child->next_sibling())
		{
			std::string nodeName = std::string(child->name());
			char* nodeValue = child->value();

			if (nodeName == MONSTERS_NAME_NODE)
				monster->setName(std::string(nodeValue));
			else if (nodeName == MONSTERS_SPRITE_NODE)
			{
				Texture* monsterTexture = new Texture(MONSTERS_DIRECTORY + std::string(nodeValue));
				monsterTexture->setReferenceCount(1);
				monster->setTexture(monsterTexture);
			}
			else if (nodeName == MONSTERS_SIZE_NODE)
				monster->getTransform()->scale(std::atoi(nodeValue) / 10.0f, std::atoi(nodeValue) / 10.0f, std::atoi(nodeValue) / 10.0f);
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