#pragma once
#include "glm\glm.hpp"

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
};

class AI
{
public:
	AI();
	~AI();
	MovementDefinition moveTo(glm::vec3 reference, glm::vec3 destination, float rangeSpeed);
	MovementDefinition moveAway(glm::vec3 reference, glm::vec3 destination, float rangeSpeed);
private:
	int randNumber;
	unsigned int randAuxiliarValue;
};

