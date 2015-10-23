#include "GameEntityFactory.h"
#include "rapidxml_utils.hpp"

using namespace rapidxml;
using namespace std;
using namespace glm;

GameEntityFactory::GameEntityFactory()
{
	//parseAllGameEntitiesInDirectory();
}


GameEntityFactory::~GameEntityFactory()
{
}

std::vector<GameEntity*> GameEntityFactory::getListOfAllGameEntities()
{
	return gameEntities;
}

GameEntity* GameEntityFactory::getGameEntityOfName(std::string name)
{
	for (GameEntity* gameEntity : gameEntities)
		if (gameEntity->getName() == name)
			return generateCopyOfGameEntity(gameEntity);

	throw GameEntityNotFoundException();
}

GameEntity* GameEntityFactory::getGameEntityOfMapColor(glm::vec3 color)
{
	for (GameEntity* gameEntity : gameEntities)
		if (gameEntity->getMapColorRed() == color.r)
			if (gameEntity->getMapColorGreen() == color.g)
				if (gameEntity->getMapColorBlue() == color.b)
					return generateCopyOfGameEntity(gameEntity);

	throw GameEntityNotFoundException();
}

bool GameEntityFactory::isGameEntityMapColorValid(glm::vec3 color)
{
	for (glm::vec3 validColor : validColors)
		if (color.r == validColor.r)
			if (color.g == validColor.g)
				if (color.b == validColor.b)
					return true;

	return false;
}

void GameEntityFactory::parseAllGameEntitiesInDirectory()
{
	for (std::string str : getListOfFilesInDirectory(GAMEENTITY_DIRECTORY))
	{
		std::string fullPath = GAMEENTITY_DIRECTORY + str;
		std::vector<char> writable(fullPath.begin(), fullPath.end());
		writable.push_back('\0');
		GameEntity* gameEntity = parseXmlGameEntity(&writable[0]);
		gameEntities.push_back(gameEntity);
	}
}

GameEntity* GameEntityFactory::generateCopyOfGameEntity(GameEntity* gameEntity)
{
	GameEntity* copy = new GameEntity();

	// Copy Basic Attributes
	copy->setMapColor(gameEntity->getMapColor());
	copy->setName(gameEntity->getName());
	// Copy Position
	copy->setPosition(gameEntity->getPosition());

	return copy;
}

GameEntity* GameEntityFactory::parseXmlGameEntity(char* monsterPath)
{
	file<> xmlFile(monsterPath);
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	GameEntity* gameEntity = new GameEntity();
	std::string xmlRootNodeName = std::string(doc.first_node()->name());

	if (xmlRootNodeName == GAMEENTITY_ROOT_NODE)
	{
		xml_node<> *rootNode = doc.first_node();

		for (xml_node<> *child = rootNode->first_node(); child; child = child->next_sibling())
		{
			std::string nodeName = std::string(child->name());
			char* nodeValue = child->value();

			gameEntity->setPosition(GAMEENTITY_STANDARD_POSITION);

			if (nodeName == GAMEENTITY_NAME_NODE)
				gameEntity->setName(std::string(nodeValue));
			else if (nodeName == GAMEENTITY_RED_NODE)
				gameEntity->setMapColorRed(std::atoi(nodeValue));
			else if (nodeName == GAMEENTITY_GREEN_NODE)
				gameEntity->setMapColorGreen(std::atoi(nodeValue));
			else if (nodeName == GAMEENTITY_BLUE_NODE)
				gameEntity->setMapColorBlue(std::atoi(nodeValue));
		}
	}

	if (!isGameEntityMapColorValid(gameEntity->getMapColor()))
		validColors.push_back(gameEntity->getMapColor());

	return gameEntity;
}