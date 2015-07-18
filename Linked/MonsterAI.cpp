#include "MonsterAI.h"
#include "Map.h"

MonsterAI::MonsterAI()
{
	virtualTravelledDistance = glm::vec3(0, 0, 0);
	this->monsterRange = STANDARD_MONSTER_RANGE;
	movingToPosition = false;
}

MonsterAI::MonsterAI(unsigned int monsterRange) : MonsterAI()
{
	this->monsterRange = monsterRange;
}

MonsterAI::~MonsterAI()
{
}

unsigned int MonsterAI::getMonsterRange()
{
	return this->monsterRange;
}

void MonsterAI::setMonsterRange(unsigned int monsterRange)
{
	this->monsterRange = monsterRange;
}

MovementDefinition MonsterAI::moveToDestination(Map *monsterMovementMap, glm::vec3 monsterMovementReference, glm::vec3 monsterMovementDestination, float monsterMovementRangeSpeed)
{
	glm::vec3 movement = glm::vec3(monsterMovementReference.x, monsterMovementReference.y, monsterMovementReference.z);
	float distance = glm::length(monsterMovementReference - monsterMovementDestination);

	// If the monster has already reached destination
//	if (distance < (monsterRange / RANGE_DIVIDER))
//	{
//		MovementDefinition movDef = MovementDefinition();
//		movDef.doMove = false;
//		stopMovingToPosition();
//		stopMovingRandomly();
//		return movDef;
//	}
	// If the monster is doing a random movement
	// Commented so position movement will have preference with regards to random movement
//	else if (isMovingRandomly())
//	{
//		// Let the random movement finishes before continue moving.
//		MovementDefinition movementDefinition = nextRandomStep();
//		stopMovingToPosition();
//		return movementDefinition;
//	}
	// If the monster is far away from destination
	/*else*/ if (distance > LIMIT_DISTANCE)
	{
		stopMovingToPosition();
		// Stop moving to position and start moving randomly.
		if (isMovingRandomly())
		{
			MovementDefinition movementDefinition = nextRandomStep();
			return movementDefinition;
		}
		else
		{
			startRandomMovement(monsterMovementMap, monsterMovementReference, monsterMovementRangeSpeed);
			MovementDefinition movementDefinition = nextRandomStep();
			return movementDefinition;
		}
	}
	// Checks if there is a collision between reference and destination
	else
	{
		// Get vector from reference to destination
		glm::vec3 differenceVector = 0.1f * glm::normalize(monsterMovementReference - monsterMovementDestination);
		// Get difference vector length
		float differenceVectorOriginalLength = (float)(glm::length(monsterMovementReference - monsterMovementDestination) - BORDER_MOVEMENT_LIMIT);

		glm::vec3 lastVec = glm::vec3(0, 0, 0);

		// Tests if there is a collision
		// If a collision is found, start moving randomly
		while (length(differenceVector) < differenceVectorOriginalLength)
		{
			differenceVector = differenceVector * 1.1f;
			
			bool sameVector = checkIfMonsterIsStillOnTheSameMapPosition(lastVec, differenceVector);
			lastVec = differenceVector;

			if (!sameVector && monsterMovementMap->coordinateHasCollision(monsterMovementReference - differenceVector))
			{
				stopMovingToPosition();
				if (isMovingRandomly())
				{
					MovementDefinition movementDefinition = nextRandomStep();
					return movementDefinition;
				}
				else
				{
					startRandomMovement(monsterMovementMap, monsterMovementReference, monsterMovementRangeSpeed);
					MovementDefinition movementDefinition = nextRandomStep();
					return movementDefinition;
				}
			}
		}
	}

	stopMovingRandomly();

	if (isMovingToPosition())
	{
		MovementDefinition movementDefinition = nextPositionMovementStep();
		return movementDefinition;
	}
	else
	{
		startPositionMovement(monsterMovementMap, monsterMovementReference, monsterMovementDestination, monsterMovementRangeSpeed);
		MovementDefinition movementDefinition = nextPositionMovementStep();
		return movementDefinition;
	}
}

void MonsterAI::startPositionMovement(Map* map, glm::vec3 reference, glm::vec3 destination, float rangeSpeed)
{
	positionMovementMap = map;
	positionMovementReference = reference;
	positionMovementDestination = destination;
	positionMovementRangeSpeed = rangeSpeed;
	virtualTravelledDistance.x = 0;
	virtualTravelledDistance.y = 0;
	virtualTravelledDistance.z = 0;
	movingToPosition = true;
}

MovementDefinition MonsterAI::nextPositionMovementStep()
{
	MovementDefinition movDef = MovementDefinition();
	movDef.doMove = false;
	glm::vec3 movement = glm::vec3(positionMovementReference.x, positionMovementReference.y, positionMovementReference.z);

	if (movingToPosition)
	{
		glm::vec3 directionVector = glm::normalize(positionMovementDestination - positionMovementReference);
		glm::vec3 moveRange = glm::vec3(directionVector.x * positionMovementRangeSpeed, directionVector.y * positionMovementRangeSpeed, 0);
		positionMovementReference = positionMovementReference + moveRange;
		virtualTravelledDistance = virtualTravelledDistance + moveRange;

		bool sameVector = checkIfMonsterIsStillOnTheSameMapPosition(movement, positionMovementReference);

		if (sameVector || !positionMovementMap->coordinateHasCollision(positionMovementReference))
		{
			movDef.doMove = true;
			movDef.movement = positionMovementReference;
			if (positionMovementDestination.x > positionMovementReference.x && positionMovementDestination.y > positionMovementReference.y)
				movDef.direction = TOP_RIGHT;
			else if (positionMovementDestination.x > positionMovementReference.x && positionMovementDestination.y < positionMovementReference.y)
				movDef.direction = BOTTOM_RIGHT;
			else if (positionMovementDestination.x < positionMovementReference.x && positionMovementDestination.y > positionMovementReference.y)
				movDef.direction = TOP_LEFT;
			else if (positionMovementDestination.x < positionMovementReference.x && positionMovementDestination.y < positionMovementReference.y)
				movDef.direction = BOTTOM_LEFT;
			else if (positionMovementDestination.x > positionMovementReference.x && positionMovementDestination.y == positionMovementReference.y)
				movDef.direction = RIGHT;
			else if (positionMovementDestination.x < positionMovementReference.x && positionMovementDestination.y == positionMovementReference.y)
				movDef.direction = LEFT;
			else if (positionMovementDestination.x == positionMovementReference.x && positionMovementDestination.y > positionMovementReference.y)
				movDef.direction = TOP;
			else if (positionMovementDestination.x == positionMovementReference.x && positionMovementDestination.y < positionMovementReference.y)
				movDef.direction = BOTTOM;
		}

		if (length(virtualTravelledDistance) >= KEEP_MOVING_FACTOR / (float)100)
			movingToPosition = false;
	}
	else
	{
		movingToPosition = true;
		virtualTravelledDistance.x = 0;
		virtualTravelledDistance.y = 0;
		virtualTravelledDistance.z = 0;
	}

	return movDef;
}

bool MonsterAI::isMovingToPosition()
{
	return movingToPosition;
}

void MonsterAI::stopMovingToPosition()
{
	this->movingToPosition = false;
}