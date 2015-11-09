#include "AI.h"
#include "Entity.h"

AI::AI(Entity& aiOwner) : aiOwner(aiOwner)
{
}


AI::~AI()
{
}

glm::vec3 AI::getNextStep(glm::vec3 destination) const
{
	glm::vec3 aiOwnerPosition = this->aiOwner.getTransform()->getPosition();

	if (aiOwnerPosition.x != destination.x && aiOwnerPosition.y != destination.y)
	{
		glm::vec3 diffVec = destination - aiOwnerPosition;
		float speedFactor = this->aiOwner.getTotalSpeed() / 100.0f;
		glm::vec3 movementVector = speedFactor * glm::normalize(diffVec);
		glm::vec3 nextStep = movementVector + aiOwnerPosition;

		return nextStep;
	}
	else
		return destination;
}

bool AI::reachDestination(glm::vec3 nextPosition, glm::vec3 destination) const
{
	glm::vec3 aiOwnerPosition = aiOwner.getTransform()->getPosition();

	if (aiOwnerPosition.x < destination.x)
	{
		if (aiOwnerPosition.y < destination.y)
		{
			if (nextPosition.x >= destination.x && nextPosition.y >= destination.y)
				return true;
			else
				return false;
		}
		else
		{
			if (nextPosition.x >= destination.x && nextPosition.y <= destination.y)
				return true;
			else
				return false;
		}
	}
	else
	{
		if (aiOwnerPosition.y < destination.y)
		{
			if (nextPosition.x <= destination.x && nextPosition.y >= destination.y)
				return true;
			else
				return false;
		}
		else
		{
			if (nextPosition.x <= destination.x && nextPosition.y <= destination.y)
				return true;
			else
				return false;
		}
	}

	return true;
}

MovementDefinition AI::getMovementDefinitionOfDestination(glm::vec3 destination)
{
	MovementDefinition movementDefinition;
	glm::vec3 aiOwnerPosition = aiOwner.getTransform()->getPosition();
	glm::vec3 diffVec = destination - aiOwnerPosition;
	movementDefinition.direction = this->getCompleteDirection(diffVec);
	movementDefinition.movement = destination;
	return movementDefinition;
}

MovementDirection AI::getCompleteDirection(glm::vec3 vector) const
{
	float angle = this->getVectorAngle(vector);

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

float AI::getVectorAngle(glm::vec3 vector) const
{
	glm::vec3 auxVector = glm::vec3(1, 0, 0);
	float radAngle = acos((glm::dot(vector, auxVector) / (glm::length(vector)*glm::length(auxVector))));
	float angle = (180 * radAngle) / PI;
	if (vector.y < 0)
		angle = -angle;
	return angle;
}