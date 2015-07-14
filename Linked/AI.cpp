#include "AI.h"
#include "Display.h"

AI::AI()
{
}


AI::~AI()
{
}

MovementDefinition AI::moveTo(glm::vec3 reference, glm::vec3 destination, float rangeSpeed)
{
	MovementDefinition movDef = MovementDefinition();

	glm::vec3 movement = glm::vec3(reference.x, reference.y, reference.z);

	if (destination.x > reference.x && destination.y > reference.y)
	{
		movement.x = movement.x + rangeSpeed;
		movement.y = movement.y + rangeSpeed;
		movDef.direction = TOP_RIGHT;
	}
	else if (destination.x > reference.x && destination.y < reference.y)
	{
		movement.x = movement.x + rangeSpeed;
		movement.y = movement.y - rangeSpeed;
		movDef.direction = BOTTOM_RIGHT;
	}
	else if (destination.x < reference.x && destination.y > reference.y)
	{
		movement.x = movement.x - rangeSpeed;
		movement.y = movement.y + rangeSpeed;
		movDef.direction = TOP_LEFT;
	}
	else if (destination.x < reference.x && destination.y < reference.y)
	{
		movement.x = movement.x - rangeSpeed;
		movement.y = movement.y - rangeSpeed;
		movDef.direction = BOTTOM_LEFT;
	}
	else if (destination.x > reference.x && destination.y == reference.y)
	{
		movement.x = movement.x + rangeSpeed;
		movDef.direction = RIGHT;
	}
	else if (destination.x < reference.x && destination.y == reference.y)
	{
		movement.x = movement.x - rangeSpeed;
		movDef.direction = LEFT;
	}
	else if (destination.x == reference.x && destination.y > reference.y)
	{
		movement.y = movement.y + rangeSpeed;
		movDef.direction = TOP;
	}
	else if (destination.x == reference.x && destination.y < reference.y)
	{
		movement.y = movement.y - rangeSpeed;
		movDef.direction = BOTTOM;
	}

	movDef.movement = movement;

	return movDef;
}

MovementDefinition AI::moveAway(glm::vec3 reference, glm::vec3 destination, float rangeSpeed)
{
	MovementDefinition movDef = MovementDefinition();

	glm::vec3 movement = glm::vec3(reference.x, reference.y, reference.z);

	if (reference.x > destination.x && reference.y > destination.y)
	{
		movement.x = movement.x + rangeSpeed;
		movement.y = movement.y + rangeSpeed;
		movDef.direction = TOP_RIGHT;
	}
	else if (reference.x > destination.x && reference.y < destination.y)
	{
		movement.x = movement.x + rangeSpeed;
		movement.y = movement.y - rangeSpeed;
		movDef.direction = BOTTOM_RIGHT;
	}
	else if (reference.x < destination.x && reference.y > destination.y)
	{
		movement.x = movement.x - rangeSpeed;
		movement.y = movement.y + rangeSpeed;
		movDef.direction = TOP_LEFT;
	}
	else if (reference.x < destination.x && reference.y < destination.y)
	{
		movement.x = movement.x - rangeSpeed;
		movement.y = movement.y - rangeSpeed;
		movDef.direction = BOTTOM_LEFT;
	}
	else if (reference.x > destination.x && reference.y == destination.y)
	{
		movement.x = movement.x + rangeSpeed;
		movDef.direction = RIGHT;
	}
	else if (reference.x < destination.x && reference.y == destination.y)
	{
		movement.x = movement.x - rangeSpeed;
		movDef.direction = LEFT;
	}
	else if (reference.x == destination.x && reference.y > destination.y)
	{
		movement.y = movement.y + rangeSpeed;
		movDef.direction = TOP;
	}
	else if (reference.x == destination.x && reference.y < destination.y)
	{
		movement.y = movement.y - rangeSpeed;
		movDef.direction = BOTTOM;
	}

	movDef.movement = movement;

	return movDef;
}