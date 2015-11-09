#include "PlayerAI.h"

PlayerAI::PlayerAI(Player& aiOwner) : aiOwner(aiOwner)
{
}


PlayerAI::~PlayerAI()
{
}

bool PlayerAI::reachDestination(glm::vec3 nextPosition, glm::vec3 destination) const
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

MovementDefinition PlayerAI::movePerfectlyTo(Map* map, glm::vec3 reference, glm::vec3 destination, float rangeSpeed) const
{
	MovementDefinition movDef = MovementDefinition();
	float distance = glm::length(glm::vec2(reference.x, reference.y) - glm::vec2(destination.x, destination.y));
	glm::vec3 movement = glm::vec3(reference.x, reference.y, reference.z);

	glm::vec3 range = rangeSpeed*glm::normalize(destination - reference);

	movement.x = movement.x + range.x;
	movement.y = movement.y + range.y;

	movDef.direction = this->getCompleteDirection(range);
	movDef.movement = movement;

	return movDef;
}

glm::vec3 PlayerAI::getNextStep(glm::vec3 destination) const
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

MovementDefinition PlayerAI::getMovementDefinitionOfDestination(glm::vec3 destination)
{
	MovementDefinition movementDefinition;
	glm::vec3 aiOwnerPosition = aiOwner.getTransform()->getPosition();
	glm::vec3 diffVec = destination - aiOwnerPosition;
	movementDefinition.direction = this->getCompleteDirection(diffVec);
	movementDefinition.movement = destination;
	return movementDefinition;
}

MovementDirection PlayerAI::getCompleteDirection(glm::vec3 vector) const
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

float PlayerAI::getVectorAngle(glm::vec3 vector) const
{
	glm::vec3 auxVector = glm::vec3(1, 0, 0);
	float radAngle = acos((glm::dot(vector, auxVector) / (glm::length(vector)*glm::length(auxVector))));
	float angle = (180 * radAngle) / PI;
	if (vector.y < 0)
		angle = -angle;
	return angle;
}