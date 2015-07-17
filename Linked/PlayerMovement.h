#pragma once
#include "glm/glm.hpp"
class Map;
class Entity;

#define SLIDE_FACTOR 20
#define SLIDE_SPEED_FACTOR 0.01f
#define PLAYER_SPEED 10

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
	bool checkIfPlayerIsStillOnTheSameMapPosition(glm::vec3 currentPosition, glm::vec3 nextPosition);
};

