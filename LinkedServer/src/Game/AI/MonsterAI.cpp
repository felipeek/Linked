#include "MonsterAI.h"
#include "Map.h"

MonsterAI::MonsterAI()
{
	positionVirtualTravelledDistance = glm::vec3(0, 0, 0);
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

	// If the monster is far away from destination
	if (distance > LIMIT_DISTANCE)
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
	positionMovementRangeSpeed = rangeSpeed;
	glm::vec3 directionVector = glm::normalize(destination - positionMovementReference);
	positionMovementMoveRange = glm::vec3(directionVector.x * positionMovementRangeSpeed, directionVector.y * positionMovementRangeSpeed, 0);
	positionMovementDirection = this->getDirectionBasedOnVector(positionMovementMoveRange);
	positionVirtualTravelledDistance.x = 0;
	positionVirtualTravelledDistance.y = 0;
	positionVirtualTravelledDistance.z = 0;
	movingToPosition = true;
}

MovementDefinition MonsterAI::nextPositionMovementStep()
{
	MovementDefinition movDef = MovementDefinition();
	movDef.doMove = false;

	if (movingToPosition)
	{
		glm::vec3 oldPositionMovementReference = positionMovementReference;
		positionMovementReference = positionMovementReference + positionMovementMoveRange;
		positionVirtualTravelledDistance = positionVirtualTravelledDistance + positionMovementMoveRange;

		bool sameVector = checkIfMonsterIsStillOnTheSameMapPosition(oldPositionMovementReference, positionMovementReference);

		if (sameVector || !positionMovementMap->coordinateHasCollision(positionMovementReference))
		{
			movDef.doMove = true;
			movDef.movement = positionMovementReference;
			movDef.direction = positionMovementDirection;
		}

		if (length(positionVirtualTravelledDistance) >= POSITION_KEEP_MOVING_FACTOR / (float)100)
			movingToPosition = false;
	}
	else
	{
		movingToPosition = true;
		positionVirtualTravelledDistance.x = 0;
		positionVirtualTravelledDistance.y = 0;
		positionVirtualTravelledDistance.z = 0;
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