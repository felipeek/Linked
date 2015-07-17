#include "GameEntityFactory.h"
#include "rapidxml_utils.hpp"
#include "Primitive.h"

using namespace rapidxml;
using namespace std;

GameEntityFactory::GameEntityFactory()
{
	parseAllGameEntitiesInDirectory();
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
	GameEntity* copy = new GameEntity(NULL, NULL, NULL);

	// Copy Basic Attributes
	copy->setMapColor(gameEntity->getMapColor());
	copy->setName(gameEntity->getName());
	// Copy Mesh (The same mesh will be setted for all gameEntities of same class)
	copy->setMesh(gameEntity->getMesh());
	// Copy Texture (A new texture object must be created for each gameEntity)
	Texture* gameEntityTexture = gameEntity->getTexture();
	copy->setTexture(new Texture(gameEntityTexture->getFilename(), 2, 2));
	// Copy Transform (A new transform object must be created for each gameEntity)
	Transform *gameEntityTransform = gameEntity->getTransform();
	vec3 gameEntityTransformPosition = gameEntityTransform->getPosition();
	vec3 gameEntityTransformScale = gameEntityTransform->getScale();
	copy->setTransform(new Transform(gameEntityTransformPosition, STANDARD_ANGLE, STANDARD_AXIS, gameEntityTransformScale));

	return copy;
}

GameEntity* GameEntityFactory::parseXmlGameEntity(char* monsterPath)
{
	file<> xmlFile(monsterPath);
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	GameEntity* gameEntity = new GameEntity(NULL, NULL, NULL);
	std::string xmlRootNodeName = std::string(doc.first_node()->name());

	if (xmlRootNodeName == ROOT_NODE)
	{
		xml_node<> *rootNode = doc.first_node();

		gameEntity->setTransform(new Transform(STANDARD_POSITION, STANDARD_ANGLE, STANDARD_AXIS, STANDARD_SCALE));

		for (xml_node<> *child = rootNode->first_node(); child; child = child->next_sibling())
		{
			std::string nodeName = std::string(child->name());
			char* nodeValue = child->value();

			if (nodeName == NAME_NODE)
				gameEntity->setName(std::string(nodeValue));
			else if (nodeName == TEXTURE_NODE)
				gameEntity->setTexture(new Texture(GAMEENTITY_DIRECTORY + std::string(nodeValue), 2, 2));
			else if (nodeName == OBJECT_NODE)
				gameEntity->setMesh(new Mesh(GAMEENTITY_DIRECTORY + std::string(nodeValue), 0, 0));
			else if (nodeName == XSIZE_NODE)
				gameEntity->getTransform()->scale(std::atoi(nodeValue) / 10.0f, 1, 1);
			else if (nodeName == YSIZE_NODE)
				gameEntity->getTransform()->scale(1, std::atoi(nodeValue) / 10.0f, 1);
			else if (nodeName == ZSIZE_NODE)
				gameEntity->getTransform()->scale(1, 1, std::atoi(nodeValue) / 10.0f);
			else if (nodeName == RED_NODE)
				gameEntity->setMapColorRed(std::atoi(nodeValue));
			else if (nodeName == GREEN_NODE)
				gameEntity->setMapColorGreen(std::atoi(nodeValue));
			else if (nodeName == BLUE_NODE)
				gameEntity->setMapColorBlue(std::atoi(nodeValue));
		}
	}

	if (!isGameEntityMapColorValid(gameEntity->getMapColor()))
		validColors.push_back(gameEntity->getMapColor());

	return gameEntity;
}