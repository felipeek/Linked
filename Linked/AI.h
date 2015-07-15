#pragma once
#include "glm\glm.hpp"
class Map;

#define LIMIT_DISTANCE 25
#define RANDOM_MOVEMENT_FACTOR 5.0f

enum MovementDirection{
	TOP,
	LEFT,
	RIGHT,
	BOTTOM,
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT
};

class MovementDefinition
{
public:
	glm::vec3 movement;
	MovementDirection direction;
	bool doMove;
};

class AI
{
public:
	AI();
	~AI();
	MovementDefinition movePerfectlyTo(Map* map, glm::vec3 reference, glm::vec3 destination, float rangeSpeed);
	MovementDefinition movePerfectlyAway(Map* map, glm::vec3 reference, glm::vec3 destination, float rangeSpeed);
	void startRandomMovement(Map* map, glm::vec3 reference, float rangeSpeed);
	MovementDefinition nextRandomStep();
	bool isMovingRandomly();
private:
	Map* randomMap;
	glm::vec3 randomReference;
	glm::vec3 randomChangedReference;
	float randomRangeSpeed;
	MovementDirection randomDirection;
	bool movingRandomly;
};

