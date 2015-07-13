#include "MonsterFactory.h"
#include "rapidxml_utils.hpp"
#include <dirent.h>
#include <iostream>
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

std::vector<Monster> MonsterFactory::getListOfAllMonsters()
{
	return monsters;
}

Monster MonsterFactory::getMonsterOfName(std::string name)
{
	for (Monster monster : monsters)
		if (monster.getName() == name)
			return monster;

	throw MonsterNotFoundException();
}

Monster MonsterFactory::getMonsterOfMapColor(glm::vec3 color)
{
	for (Monster monster : monsters)
		if (monster.getMapColorRed() == color.r)
			if (monster.getMapColorGreen() == color.g)
				if (monster.getMapColorBlue() == color.b)
					return monster;

	throw MonsterNotFoundException();
}

void MonsterFactory::parseAllMonstersInDirectory()
{
	for (std::string str : getListOfFilesInDirectory())
	{
		std::string fullPath = MONSTERS_DIRECTORY + str;
		std::vector<char> writable(fullPath.begin(), fullPath.end());
		writable.push_back('\0');
		Monster monster = parseXmlMonster(&writable[0]);
		monsters.push_back(monster);
	}
}

Monster MonsterFactory::parseXmlMonster(char* monsterPath)
{
	file<> xmlFile(monsterPath);
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	Monster monster(NULL, NULL);
	std::string xmlRootNodeName = std::string(doc.first_node()->name());

	if (xmlRootNodeName == ROOT_NODE)
	{
		xml_node<> *rootNode = doc.first_node();

		for (xml_node<> *child = rootNode->first_node(); child; child = child->next_sibling())
		{
			std::string nodeName = std::string(child->name());
			char* nodeValue = child->value();
				
			if (nodeName == NAME_NODE)
				monster.setName(std::string(nodeValue));
			else if (nodeName == HP_NODE)
				monster.setHp(std::atoi(nodeValue));
			else if (nodeName == ATTACK_NODE)
				monster.setAttack(std::atoi(nodeValue));
			else if (nodeName == DEFENSE_NODE)
				monster.setDefense(std::atoi(nodeValue));
			else if (nodeName == RED_NODE)
				monster.setMapColorRed(std::atoi(nodeValue));
			else if (nodeName == GREEN_NODE)
				monster.setMapColorBlue(std::atoi(nodeValue));
			else if (nodeName == BLUE_NODE)
				monster.setMapColorBlue(std::atoi(nodeValue));
		}
	}

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