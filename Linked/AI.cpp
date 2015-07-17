#include "AI.h"
#include "Display.h"
#include "Map.h"
#include "Time.h"
#include <stdlib.h>
#include <time.h>

AI::AI()
{
	movingRandomly = false;
}

AI::~AI()
{
}

bool AI::isMovingRandomly()
{
	return this->movingRandomly;
}

void AI::stopMovingRandomly()
{
	this->movingRandomly = false;
}

void AI::startRandomMovement(Map* map, glm::vec3 reference, float rangeSpeed)
{
	this->randomMap = map;
	randomReference = reference;
	randomChangedReference = reference;
	randomRangeSpeed = rangeSpeed;
	timeRandomMovementStarted = Time::getTime();

	int randNumber = rand() % 8;

	switch (randNumber)
	{
	case 0:
		randomDirection = TOP_RIGHT; break;
	case 1:
		randomDirection = RIGHT; break;
	case 2:
		randomDirection = BOTTOM_RIGHT; break;
	case 3:
		randomDirection = BOTTOM; break;
	case 4:
		randomDirection = BOTTOM_LEFT; break;
	case 5:
		randomDirection = LEFT; break;
	case 6:
		randomDirection = TOP_LEFT; break;
	case 7:
		randomDirection = TOP; break;
	default:
		randomDirection = TOP_RIGHT; break;
	}

	this->movingRandomly = true;
}

MovementDefinition AI::nextRandomStep()
{
	MovementDefinition movDef = MovementDefinition();
	bool shouldNotBeMoving = (Time::getTime() - timeRandomMovementStarted) <= (STAND_STILL_RANDOM_FACTOR / (float)10);

	if (!movingRandomly || shouldNotBeMoving)
	{
		movDef.doMove = false;
		return movDef;
	}

	switch (randomDirection)
	{
		case TOP_RIGHT:
			randomChangedReference.x = randomChangedReference.x + randomRangeSpeed;
			randomChangedReference.y = randomChangedReference.y + randomRangeSpeed;
			break;
		case RIGHT:
			randomChangedReference.x = randomChangedReference.x + randomRangeSpeed;
			break;
		case BOTTOM_RIGHT:
			randomChangedReference.x = randomChangedReference.x + randomRangeSpeed;
			randomChangedReference.y = randomChangedReference.y - randomRangeSpeed;
			break;
		case BOTTOM:
			randomChangedReference.y = randomChangedReference.y - randomRangeSpeed;
			break;
		case BOTTOM_LEFT:
			randomChangedReference.x = randomChangedReference.x - randomRangeSpeed;
			randomChangedReference.y = randomChangedReference.y - randomRangeSpeed;
			break;
		case LEFT:
			randomChangedReference.x = randomChangedReference.x - randomRangeSpeed;
			break;
		case TOP_LEFT:
			randomChangedReference.x = randomChangedReference.x - randomRangeSpeed;
			randomChangedReference.y = randomChangedReference.y + randomRangeSpeed;
			break;
		case TOP:
			randomChangedReference.y = randomChangedReference.y + randomRangeSpeed;
			break;
		default:
			break;
	}

	movDef.direction = randomDirection;
	movDef.movement = randomChangedReference;

	if (!MapTerrainImageLoader::isOfCollisionType(randomMap->getMapTerrainWithMovementCollisionForCoordinate(randomChangedReference)))
	{
		movDef.doMove = true;
		if (length(randomReference - randomChangedReference) >= RANDOM_MOVEMENT_FACTOR)
			this->movingRandomly = false;
	}
	else
	{
		movDef.doMove = false;
		this->movingRandomly = false;
	}

	return movDef;
}

MovementDefinition AI::movePerfectlyTo(Map* map, glm::vec3 reference, glm::vec3 destination, float rangeSpeed)
{
	MovementDefinition movDef = MovementDefinition();
	movDef.doMove = false;
	float distance = glm::length(reference - destination);
	glm::vec3 movement = glm::vec3(reference.x, reference.y, reference.z);

	if (distance < LIMIT_DISTANCE && !isMovingRandomly())
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

		if (!MapTerrainImageLoader::isOfCollisionType(map->getMapTerrainWithMovementCollisionForCoordinate(movement)))
		{
			movDef.doMove = true;
			movDef.movement = movement;
		}
	}
	else
	{
		if (isMovingRandomly())
		{
			return nextRandomStep();
		}
		else
		{
			startRandomMovement(map, reference, rangeSpeed);
			return nextRandomStep();
		}
	}

	return movDef;
}

MovementDefinition AI::movePerfectlyAway(Map* map, glm::vec3 reference, glm::vec3 destination, float rangeSpeed)
{
	MovementDefinition movDef = MovementDefinition();
	movDef.doMove = false;
	float distance = glm::length(reference - destination);
	glm::vec3 movement = glm::vec3(reference.x, reference.y, reference.z);
	
	if (distance < LIMIT_DISTANCE && !isMovingRandomly())
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

		if (!MapTerrainImageLoader::isOfCollisionType(map->getMapTerrainWithMovementCollisionForCoordinate(movement)))
		{
			movDef.doMove = true;
			movDef.movement = movement;
		}
	}
	else
	{
		if (isMovingRandomly())
		{
			return nextRandomStep();
		}
		else
		{
			startRandomMovement(map, reference, rangeSpeed);
			return nextRandomStep();
		}
	}

	return movDef;
}