#include "PlayerAI.h"

PlayerAI::PlayerAI(Player& aiOwner) : AI(aiOwner)
{
}


PlayerAI::~PlayerAI()
{
}

glm::vec3 PlayerAI::getNextStep(glm::vec3 destination) const
{
	Player& aiOwner = (Player&)this->getAiOwner();
	glm::vec3 aiOwnerPosition = aiOwner.getTransform()->getPosition();

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