#include "MonsterFactory.h"
#include "rapidxml_utils.hpp"
#include <dirent.h>
#include <iostream>
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
	for (std::string str : getListOfFilesInDirectory())
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
	file<> xmlFile(monsterPath);
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	Monster* monster = new Monster(NULL, NULL, NULL);
	std::string xmlRootNodeName = std::string(doc.first_node()->name());

	if (xmlRootNodeName == ROOT_NODE)
	{
		xml_node<> *rootNode = doc.first_node();

		for (xml_node<> *child = rootNode->first_node(); child; child = child->next_sibling())
		{
			std::string nodeName = std::string(child->name());
			char* nodeValue = child->value();
			
			Mesh *mesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f));
			monster->setMesh(mesh);

			if (nodeName == NAME_NODE)
				monster->setName(std::string(nodeValue));
			else if (nodeName == SPRITE_NODE)
				monster->setTexture(new Texture(MONSTERS_DIRECTORY + std::string(nodeValue), 2, 2));
			else if (nodeName == HP_NODE)
				monster->setHp(std::atoi(nodeValue));
			else if (nodeName == ATTACK_NODE)
				monster->setAttack(std::atoi(nodeValue));
			else if (nodeName == DEFENSE_NODE)
				monster->setDefense(std::atoi(nodeValue));
			else if (nodeName == RED_NODE)
				monster->setMapColorRed(std::atoi(nodeValue));
			else if (nodeName == GREEN_NODE)
				monster->setMapColorGreen(std::atoi(nodeValue));
			else if (nodeName == BLUE_NODE)
				monster->setMapColorBlue(std::atoi(nodeValue));
		}
	}

	if (!isMonsterMapColorValid(monster->getMapColor()))
		validColors.push_back(monster->getMapColor());

	return monster;
}

std::vector<std::string> MonsterFactory::getListOfFilesInDirectory()
{
	DIR *dir;
	struct dirent *ent;
	char* aux;
	std::vector<std::string> fileNames;

	if ((dir = opendir(MONSTERS_DIRECTORY)) != NULL)
	{
		while ((ent = readdir(dir)) != NULL) {
			aux = ent->d_name;
			if (strstr(aux, FILE_EXTENSION))
				fileNames.push_back(std::string(aux));
		}

		closedir(dir);
	}
	else
	{
		perror(READ_DIRECTORY_ERROR);
	}

	return fileNames;
}

Monster* MonsterFactory::generateCopyOfMonster(Monster* monster)
{
	Monster* copy = new Monster(NULL, NULL, NULL);
	copy->setTexture(monster->getTexture());
	copy->setAttack(monster->getAttack());
	copy->setDefense(monster->getDefense());
	copy->setHp(monster->getHp());
	copy->setMapColor(monster->getMapColor());
	copy->setMesh(monster->getMesh());
	copy->setName(monster->getName());
	copy->setTransform(monster->getTransform());
	return copy;
}