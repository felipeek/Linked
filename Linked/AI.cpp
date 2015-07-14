#include "AI.h"
#include "Display.h"
#include <stdlib.h>
#include <time.h>

AI::AI()
{
	randAuxiliarValue = RANDOM_MOVEMENT_FACTOR;
}


AI::~AI()
{
}

MovementDefinition AI::moveTo(glm::vec3 reference, glm::vec3 destination, float rangeSpeed)
{
	MovementDefinition movDef = MovementDefinition();
	float distance = glm::length(reference - destination);
	glm::vec3 movement = glm::vec3(reference.x, reference.y, reference.z);

	if (distance < LIMIT_DISTANCE && randAuxiliarValue == RANDOM_MOVEMENT_FACTOR)
	{
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
	}
	else
	{
		if (randAuxiliarValue >= RANDOM_MOVEMENT_FACTOR)
		{
			randNumber = rand() % 8;
			randAuxiliarValue = 0;
		}
		else
			randAuxiliarValue++;

		switch (randNumber)
		{
		case 0:
			movement.x = movement.x + rangeSpeed;
			movement.y = movement.y + rangeSpeed;
			movDef.direction = TOP_RIGHT;
			break;
		case 1:
			movement.x = movement.x + rangeSpeed;
			movement.y = movement.y - rangeSpeed;
			movDef.direction = BOTTOM_RIGHT;
			break;
		case 2:
			movement.x = movement.x - rangeSpeed;
			movement.y = movement.y + rangeSpeed;
			movDef.direction = TOP_LEFT;
			break;
		case 3:
			movement.x = movement.x - rangeSpeed;
			movement.y = movement.y - rangeSpeed;
			movDef.direction = BOTTOM_LEFT;
			break;
		case 4:
			movement.x = movement.x + rangeSpeed;
			movDef.direction = RIGHT;
			break;
		case 5:
			movement.x = movement.x - rangeSpeed;
			movDef.direction = LEFT;
			break;
		case 6:
			movement.y = movement.y + rangeSpeed;
			movDef.direction = TOP;
			break;
		case 7:
			movement.y = movement.y - rangeSpeed;
			movDef.direction = BOTTOM;
			break;
		}
	}

	movDef.movement = movement;

	return movDef;
}

MovementDefinition AI::moveAway(glm::vec3 reference, glm::vec3 destination, float rangeSpeed)
{
	MovementDefinition movDef = MovementDefinition();
	float distance = glm::length(reference - destination);
	glm::vec3 movement = glm::vec3(reference.x, reference.y, reference.z);
	
	if (distance < LIMIT_DISTANCE && randAuxiliarValue == RANDOM_MOVEMENT_FACTOR)
	{
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
	}
	else
	{
		if (randAuxiliarValue >= RANDOM_MOVEMENT_FACTOR)
		{
			randNumber = rand() % 8;
			randAuxiliarValue = 0;
		}
		else
			randAuxiliarValue++;

		switch (randNumber)
		{
		case 0:
			movement.x = movement.x + rangeSpeed;
			movement.y = movement.y + rangeSpeed;
			movDef.direction = TOP_RIGHT;
			break;
		case 1:
			movement.x = movement.x + rangeSpeed;
			movement.y = movement.y - rangeSpeed;
			movDef.direction = BOTTOM_RIGHT;
			break;
		case 2:
			movement.x = movement.x - rangeSpeed;
			movement.y = movement.y + rangeSpeed;
			movDef.direction = TOP_LEFT;
			break;
		case 3:
			movement.x = movement.x - rangeSpeed;
			movement.y = movement.y - rangeSpeed;
			movDef.direction = BOTTOM_LEFT;
			break;
		case 4:
			movement.x = movement.x + rangeSpeed;
			movDef.direction = RIGHT;
			break;
		case 5:
			movement.x = movement.x - rangeSpeed;
			movDef.direction = LEFT;
			break;
		case 6:
			movement.y = movement.y + rangeSpeed;
			movDef.direction = TOP;
			break;
		case 7:
			movement.y = movement.y - rangeSpeed;
			movDef.direction = BOTTOM;
			break;
		}
	}

	movDef.movement = movement;

	return movDef;
}