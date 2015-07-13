#pragma once
#include "glm/glm.hpp"
#include "Map.h"
#include "Entity.h"

#define SLIDE_FACTOR 60
#define SLIDE_SPEED_FACTOR 0.01f
#define PLAYER_SPEED 15

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
	PlayerMovement(Map* map, Entity* player);
	~PlayerMovement();
	void inputPlayerMovement();
private:
	Map* map;
	Entity* player;
	bool moveTo(MovementDirection direction, glm::vec3 currentPosition, float range, bool isOnlyKeyPressed, glm::vec3 *endPosition);
};

