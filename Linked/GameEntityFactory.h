#pragma once
#include "GameEntity.h"
#include "Factory.h"

#define GAMEENTITY_DIRECTORY "./res/GameEntities/"
#define ROOT_NODE "GAMEENTITY"
#define NAME_NODE "NAME"
#define OBJECT_NODE "OBJECT"
#define TEXTURE_NODE "TEXTURE"
#define XSIZE_NODE "XSIZE"
#define YSIZE_NODE "YSIZE"
#define ZSIZE_NODE "ZSIZE"
#define RED_NODE "RED"
#define GREEN_NODE "GREEN"
#define BLUE_NODE "BLUE"
#define STANDARD_POSITION vec3(0, 0, 1.0f)
#define STANDARD_ANGLE 35
#define STANDARD_AXIS vec3(1, 0, 0)
#define STANDARD_SCALE vec3(1, 1, 1)

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

