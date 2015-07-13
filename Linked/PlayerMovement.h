#pragma once
#include "glm/glm.hpp"
#include "Map.h"

enum MovementDirection
{
	NORTH,
	WEST,
	EAST,
	SOUTH
};

class PlayerMovement
{
public:
	PlayerMovement(Map* map);
	~PlayerMovement();
	bool moveTo(MovementDirection direction, glm::vec3 currentPosition, float range, bool isOnlyKeyPressed, glm::vec3 *endPosition);
private:
	Map* map;
};

