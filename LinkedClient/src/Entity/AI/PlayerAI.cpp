#include "PlayerAI.h"

PlayerAI::PlayerAI(Player& aiOwner) : aiOwner(aiOwner)
{
}


PlayerAI::~PlayerAI()
{
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