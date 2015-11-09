#include "AI.h"
#include "Entity.h"

AI::AI(Entity& aiOwner) : aiOwner(aiOwner)
{
}


AI::~AI()
{
}

Entity& AI::getAiOwner() const
{
	return this->aiOwner;
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

MovementDefinition AI::getMovementDefinitionOfDestination(glm::vec3 destination, bool diagonalDirection)
{
	MovementDefinition movementDefinition;
	glm::vec3 aiOwnerPosition = aiOwner.getTransform()->getPosition();
	glm::vec3 diffVec = destination - aiOwnerPosition;
	if (diagonalDirection)
		movementDefinition.direction = this->getDiagonalDirection(diffVec);
	else
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

MovementDirection AI::getDiagonalDirection(glm::vec3 vector) const
{
	float angle = this->getVectorAngle(vector);

	if (angle >= 0 && angle < 90)
		return TOP_RIGHT;
	else if (angle >= 90 && angle <= 180)
		return TOP_LEFT;
	else if (angle >= -180 && angle < -90)
		return BOTTOM_LEFT;
	else if (angle >= -90 && angle < 0)
		return BOTTOM_RIGHT;

	return TOP_RIGHT;
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