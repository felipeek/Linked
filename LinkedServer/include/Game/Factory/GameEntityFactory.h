#pragma once
#include "GameEntity.h"
#include "Factory.h"

#define GAMEENTITY_DIRECTORY "./res/GameEntities/"
#define GAMEENTITY_ROOT_NODE "GAMEENTITY"
#define GAMEENTITY_NAME_NODE "NAME"
#define GAMEENTITY_RED_NODE "RED"
#define GAMEENTITY_GREEN_NODE "GREEN"
#define GAMEENTITY_BLUE_NODE "BLUE"
#define GAMEENTITY_STANDARD_POSITION vec3(0, 0, 0)

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

