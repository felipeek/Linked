#pragma once
#include "Factory.h"
#include <glm\glm.hpp>

#define GAMEENTITY_DIRECTORY "./res/GameEntities/"
#define GAMEENTITY_ROOT_NODE "GAMEENTITY"
#define GAMEENTITY_NAME_NODE "NAME"
#define GAMEENTITY_OBJECT_NODE "OBJECT"
#define GAMEENTITY_TEXTURE_NODE "TEXTURE"
#define GAMEENTITY_SIZE_NODE "SIZE"
#define GAMEENTITY_ANGLE_NODE "ANGLE"
#define GAMEENTITY_RED_NODE "RED"
#define GAMEENTITY_GREEN_NODE "GREEN"
#define GAMEENTITY_BLUE_NODE "BLUE"
#define GAMEENTITY_STANDARD_POSITION glm::vec3(0, 0, 0)
#define GAMEENTITY_STANDARD_ANGLE 90
#define GAMEENTITY_STANDARD_AXIS glm::vec3(1, 0, 0)
#define GAMEENTITY_STANDARD_SCALE glm::vec3(1, 1, 1)

class GameEntity;

class GameEntityNotFoundException : std::exception{};

class GameEntityFactory : public Factory
{
public:
	GameEntityFactory();
	~GameEntityFactory();
	std::vector<GameEntity*> getListOfAllGameEntities();
	// Obs.: This function will generate a new GameEntity in the heap.
	// Memory must be managed carefully.
	GameEntity* getGameEntityOfName(std::string name);
	// Obs.: This function will generate a new Monster in the heap.
	// Memory must be managed carefully.
	GameEntity* getGameEntityOfMapColor(glm::vec3 color);
	bool isGameEntityMapColorValid(glm::vec3 color);
private:
	GameEntity* parseXmlGameEntity(char* gameEntityPath);
	void parseAllGameEntitiesInDirectory();
	std::vector<GameEntity*> gameEntities;
	std::vector<glm::vec3> validColors;
	GameEntity* generateCopyOfGameEntity(GameEntity* gameEntity);
};

