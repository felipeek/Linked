#include "GameEntityFactory.h"
#include "rapidxml_utils.hpp"
#include "Primitive.h"
#include <iostream>

using namespace rapidxml;
using namespace std;

GameEntityFactory::GameEntityFactory()
{
	parseAllGameEntitiesInDirectory();
}


GameEntityFactory::~GameEntityFactory()
{
	for (int i = 0, size = gameEntities.size(); i < size; i++)
	{
		delete gameEntities[0];
		gameEntities.erase(gameEntities.begin());
	}
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
	GameEntity* copy = new GameEntity(NULL, NULL, NULL);
	
	// Copy Basic Attributes
	copy->setMapColor(gameEntity->getMapColor());
	copy->setName(gameEntity->getName());
	// Copy Mesh (The same mesh will be setted for all gameEntities of same class)
	copy->setMesh(gameEntity->getMesh());
	// Copy Texture (A new texture object must be created for each gameEntity)
	copy->setTexture(gameEntity->getTexture());
	// Copy Transform (A new transform object must be created for each gameEntity)
	Transform *gameEntityTransform = gameEntity->getTransform();
	glm::vec3 gameEntityTransformPosition = gameEntityTransform->getPosition();
	glm::vec3 gameEntityTransformScale = gameEntityTransform->getScale();
	float gameEntityTransformAngleY = gameEntityTransform->getAngleY();
	copy->setTransform(new Transform(gameEntityTransformPosition, GAMEENTITY_STANDARD_ANGLE, GAMEENTITY_STANDARD_AXIS, gameEntityTransformScale));
	copy->getTransform()->incRotateY(gameEntityTransformAngleY);
	
	return copy;
}

GameEntity* GameEntityFactory::parseXmlGameEntity(char* monsterPath)
{
	file<> xmlFile(monsterPath);
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	GameEntity* gameEntity = new GameEntity(NULL, NULL, NULL);
	std::string xmlRootNodeName = std::string(doc.first_node()->name());

	if (xmlRootNodeName == GAMEENTITY_ROOT_NODE)
	{
		xml_node<> *rootNode = doc.first_node();

		gameEntity->setTransform(new Transform(GAMEENTITY_STANDARD_POSITION, GAMEENTITY_STANDARD_ANGLE, GAMEENTITY_STANDARD_AXIS, GAMEENTITY_STANDARD_SCALE));

		for (xml_node<> *child = rootNode->first_node(); child; child = child->next_sibling())
		{
			std::string nodeName = std::string(child->name());
			char* nodeValue = child->value();

			if (nodeName == GAMEENTITY_NAME_NODE)
				gameEntity->setName(std::string(nodeValue));
			else if (nodeName == GAMEENTITY_TEXTURE_NODE)
				gameEntity->setTexture(new Texture(GAMEENTITY_DIRECTORY + std::string(nodeValue)));
			else if (nodeName == GAMEENTITY_OBJECT_NODE)
				gameEntity->setMesh(new Mesh(GAMEENTITY_DIRECTORY + std::string(nodeValue), 0, 0));
			else if (nodeName == GAMEENTITY_SIZE_NODE)
				gameEntity->getTransform()->scale(std::atoi(nodeValue) / 10.0f, std::atoi(nodeValue) / 10.0f, std::atoi(nodeValue) / 10.0f);
			else if (nodeName == GAMEENTITY_ANGLE_NODE)
				gameEntity->getTransform()->incRotateY(std::stof(nodeValue));
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