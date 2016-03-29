#include "MonsterAI.h"
#include "Map.h"
#include "Monster.h"
#include "LinkedTime.h"

MonsterAI::MonsterAI(Monster& aiOwner) : AI(aiOwner)
{
}

MonsterAI::~MonsterAI()
{
}

glm::vec3 MonsterAI::getNextStep(glm::vec3 destination) const
{
	Monster& aiOwner = (Monster&)this->getAiOwner();
	glm::vec3 aiOwnerPosition = this->getAiOwner().getTransform()->getPosition();

	if (aiOwnerPosition.x != destination.x || aiOwnerPosition.y != destination.y)
	{
		glm::vec3 diffVec = destination - aiOwnerPosition;
		float speedFactor = aiOwner.getTotalSpeed() / 100.0f;
		glm::vec3 movementVector = speedFactor * glm::normalize(diffVec);
		glm::vec3 nextStep = movementVector + aiOwnerPosition;

		return nextStep;
	}
	else
		return destination;
}

MovementDefinition MonsterAI::generateRandomMovement(Map* map) const
{
	glm::vec3 aiOwnerPosition = this->getAiOwner().getTransform()->getPosition();
	MovementDefinition randomMovement;
	int xRandNumber = 0;
	int yRandNumber = 0;
	int tries = 0;

	do
	{
		do
		{
			xRandNumber = rand() % 1001;
			yRandNumber = rand() % 1001;
		} while (xRandNumber == 0 && yRandNumber == 0);

		if (rand() % 2)	xRandNumber = -xRandNumber;
		if (rand() % 2)	yRandNumber = -yRandNumber;

		glm::vec3 normalizedDirectionVector = glm::normalize(glm::vec3(xRandNumber, yRandNumber, 0));
		glm::vec3 randomMovementDirection = (float)RANDOM_KEEP_MOVING_FACTOR * glm::vec3(normalizedDirectionVector.x, normalizedDirectionVector.y, 0);
		randomMovement.direction = this->getDiagonalDirection(randomMovementDirection);
		randomMovement.movement = randomMovementDirection + aiOwnerPosition;
		tries++;
	}
	while (map->coordinateHasCollision(this->getNextStep(randomMovement.movement)) && tries < AVOID_WALLS_INTELLIGENCE);

	return randomMovement;
}

MovementDefinition MonsterAI::generateMovementTowardsCoordinate(glm::vec3 destination) const
{
	glm::vec3 aiOwnerPosition = this->getAiOwner().getTransform()->getPosition();
	glm::vec3 pathVector = destination - aiOwnerPosition;
	float pathVectorLength = glm::length(pathVector);

	if (pathVectorLength > (float)DIRECTED_KEEP_MOVING_FACTOR)
		pathVector = (float)DIRECTED_KEEP_MOVING_FACTOR*glm::normalize(pathVector);

	MovementDefinition directedMovement;
	directedMovement.movement = aiOwnerPosition + pathVector;
	directedMovement.direction = this->getDiagonalDirection(pathVector);

	return directedMovement;
}

bool MonsterAI::isOnRangeToAttack(glm::vec3 worldObjectPosition) const
{
	Monster& aiOwner = (Monster&)this->getAiOwner();
	glm::vec3 aiOwnerPosition = this->getAiOwner().getTransform()->getPosition();
	glm::vec3 pathVector = worldObjectPosition - aiOwnerPosition;
	float pathVectorLength = glm::length(pathVector);
	
	return (pathVectorLength < (float)aiOwner.getTotalRange()/RANGE_ATTACK_FACTOR) ? true : false;
}

bool MonsterAI::isOnRangeToChaseTarget(glm::vec3 worldObjectPosition) const
{
	glm::vec3 aiOwnerPosition = this->getAiOwner().getTransform()->getPosition();
	glm::vec3 pathVector = worldObjectPosition - aiOwnerPosition;
	float pathVectorLength = glm::length(pathVector);
	return (pathVectorLength < (float)RANGE_CHASE_TARGET) ? true : false;
}

// Crazy algorithm. Don't try to understand it.
bool MonsterAI::isPathFreeOfCollisions(Map* map, glm::vec3 destination) const
{
	glm::vec3 aiOwnerPosition = this->getAiOwner().getTransform()->getPosition();
	float xPos1, xPos2, yPos1, yPos2;

	if (aiOwnerPosition.x < destination.x)
	{
		xPos1 = aiOwnerPosition.x;
		xPos2 = destination.x;
		yPos1 = aiOwnerPosition.y;
		yPos2 = destination.y;
	}
	else
	{
		xPos1 = destination.x;
		xPos2 = aiOwnerPosition.x;
		yPos1 = destination.y;
		yPos2 = aiOwnerPosition.y;
	}

	float leftContribution = (floor(xPos1) + 1.0f) - xPos1;
	float rightContribution = xPos2 - floor(xPos2);

	int numberOfElementsInTheMiddleRange = abs((int)ceil(xPos1) - (int)floor(xPos2));

	float n = 1.0f / (leftContribution + rightContribution + (float)numberOfElementsInTheMiddleRange);

	float realLeftContribution = n * leftContribution;
	float realRightContribution = n * rightContribution;
	float realMiddleContribution = n;

	float yDifference = yPos2 - yPos1;

	float leftRange = realLeftContribution * yDifference;
	float rightRange = realRightContribution * yDifference;
	float middleRange = realMiddleContribution * yDifference;

	float nextLeftRange = leftRange + yPos1;

	if (yPos2 > yPos1)
	{
		for (int i = (int)floor(yPos1); i <= floor(nextLeftRange); i++)
			if (map->coordinateHasCollision(glm::vec3(floor(xPos1), i, 0)))
				return false;

		for (int i = 0; i < numberOfElementsInTheMiddleRange; i++, nextLeftRange = nextLeftRange + middleRange)
			for (int j = (int)floor(nextLeftRange); j <= floor(nextLeftRange + middleRange); j++)
				if (map->coordinateHasCollision(glm::vec3(floor(xPos1 + i + 1), j, 0)))
					return false;

		for (int i = (int)floor(nextLeftRange); i <= floor(nextLeftRange + rightRange); i++)
			if (map->coordinateHasCollision(glm::vec3(floor(xPos2), i, 0)))
				return false;
	}
	else
	{
		for (int i = (int)floor(yPos1); i >= floor(nextLeftRange); i--)
			if (map->coordinateHasCollision(glm::vec3(floor(xPos1), i, 0)))
				return false;

		for (int i = 0; i < numberOfElementsInTheMiddleRange; i++, nextLeftRange = nextLeftRange + middleRange)
			for (int j = (int)floor(nextLeftRange); j >= floor(nextLeftRange + middleRange); j--)
				if (map->coordinateHasCollision(glm::vec3(floor(xPos1 + i + 1), j, 0)))
					return false;

		for (int i = (int)floor(nextLeftRange); i >= floor(nextLeftRange + rightRange); i--)
			if (map->coordinateHasCollision(glm::vec3(floor(xPos2), i, 0)))
				return false;
	}

	return true;
}

bool MonsterAI::shouldStandStill() const
{
	double now = LinkedTime::getTime();
	if ((now - this->standStillTime) < STAND_STILL_TIME)
		return true;
	return false;
}

void MonsterAI::resetStandStill()
{
	this->standStillTime = LinkedTime::getTime();
}