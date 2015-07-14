#pragma once
#include "glm\glm.hpp"
class Map;

#define LIMIT_DISTANCE 25
#define RANDOM_MOVEMENT_FACTOR 100

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
	MovementDefinition moveTo(Map* map, glm::vec3 reference, glm::vec3 destination, float rangeSpeed);
	MovementDefinition moveAway(Map* map, glm::vec3 reference, glm::vec3 destination, float rangeSpeed);
private:
	MovementDefinition moveToRandomPosition(Map* map, glm::vec3 reference, glm::vec3 destination, float rangeSpeed);
	int randNumber;
	unsigned int randAuxiliarValue;
};

