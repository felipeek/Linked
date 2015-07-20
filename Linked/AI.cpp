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
	int xRandNumber = rand() % 1001;
	int yRandNumber = rand() % 1001;

	if (rand() % 2)	xRandNumber = -xRandNumber;
	if (rand() % 2)	yRandNumber = -yRandNumber;

	randomMovementMap = map;
	randomMovementReference = reference;
	randomMovementRangeSpeed = rangeSpeed;
	glm::vec3 directionVector = glm::normalize(glm::vec3(xRandNumber, yRandNumber, 0));
	randomMovementMoveRange = glm::vec3(directionVector.x * rangeSpeed, directionVector.y * rangeSpeed, 0);
	randomMovementDirection = this->getDirectionBasedOnVector(randomMovementMoveRange);
	timeRandomMovementStarted = Time::getTime();
	randomVirtualTravelledDistance.x = 0;
	randomVirtualTravelledDistance.y = 0;
	randomVirtualTravelledDistance.z = 0;
	randomStandStillFactor = (rand() % (STAND_STILL_RANDOM_FACTOR_MAXIMUM - STAND_STILL_RANDOM_FACTOR_MINIMUM + 1)) + STAND_STILL_RANDOM_FACTOR_MINIMUM;
	movingRandomly = true;
}

MovementDefinition AI::nextRandomStep()
{
	MovementDefinition movDef = MovementDefinition();
	movDef.doMove = false;

	bool shouldNotBeMoving = (Time::getTime() - timeRandomMovementStarted) <= (randomStandStillFactor / (float)10);

	if (shouldNotBeMoving)
		return movDef;

	if (movingRandomly)
	{
		glm::vec3 oldRandomMovementReference = randomMovementReference;
		randomMovementReference = randomMovementReference + randomMovementMoveRange;
		randomVirtualTravelledDistance = randomVirtualTravelledDistance + randomMovementMoveRange;

		bool sameVector = checkIfMonsterIsStillOnTheSameMapPosition(oldRandomMovementReference, randomMovementReference);

		if (sameVector || !randomMovementMap->coordinateHasCollision(randomMovementReference))
		{
			movDef.doMove = true;
			movDef.movement = randomMovementReference;
			movDef.direction = randomMovementDirection;

			if (length(randomVirtualTravelledDistance) >= RANDOM_KEEP_MOVING_FACTOR / (float)100)
				movingRandomly = false;
		}
		else
		{
			movDef.doMove = false;
			movingRandomly = false;
		}
	}
	else
	{
		movingRandomly = true;
		randomVirtualTravelledDistance.x = 0;
		randomVirtualTravelledDistance.y = 0;
		randomVirtualTravelledDistance.z = 0;
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

		bool samePosition = checkIfMonsterIsStillOnTheSameMapPosition(reference, movement);

		if (samePosition || !map->coordinateHasCollision(movement))
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

		bool samePosition = checkIfMonsterIsStillOnTheSameMapPosition(reference, movement);

		if (samePosition || !map->coordinateHasCollision(movement))
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

bool AI::checkIfMonsterIsStillOnTheSameMapPosition(glm::vec3 currentPosition, glm::vec3 nextPosition)
{
	if (floor(currentPosition.x) == floor(nextPosition.x))
		if (floor(currentPosition.y) == floor(nextPosition.y))
			if (floor(currentPosition.z) == floor(nextPosition.z))
				return true;

	return false;
}

MovementDirection AI::getDirectionBasedOnVector(glm::vec3 vector)
{
	glm::vec3 auxVector = glm::vec3(1, 0, 0);
	float radAngle = acos((glm::dot(vector, auxVector) / (length(vector)*length(auxVector))));
	float angle = (180 * radAngle) / PI;
	if (vector.y < 0)
		angle = -angle;

	if (angle >= -22.5f && angle < 22.5f)
		return RIGHT;
	else if (angle >= 22.5f && angle < 67.5f)
		return TOP_RIGHT;
	else if (angle >= 67.5f && angle < 112.5f)
		return TOP;
	else if (angle >= 112.5f && angle < 157.5f)
		return TOP_LEFT;
	else if (angle >= 157.5f && angle <= 180.0f || angle >= -180.0f && angle < -157.5f)
		return LEFT;
	else if (angle >= -157.5f && angle < -112.5f)
		return BOTTOM_LEFT;
	else if (angle >= -112.5f && angle < -67.5f)
		return BOTTOM;
	else if (angle >= -67.5f && angle < -22.5f)
		return BOTTOM_RIGHT;

	return RIGHT;
}