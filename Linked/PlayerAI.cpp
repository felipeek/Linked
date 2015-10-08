#include "PlayerAI.h"
#include "Map.h"

PlayerAI::PlayerAI()
{
}


PlayerAI::~PlayerAI()
{
}

MovementDefinition PlayerAI::movePerfectlyTo(Map* map, glm::vec3 reference, glm::vec3 destination, float rangeSpeed)
{
	MovementDefinition movDef = MovementDefinition();
	movDef.doMove = false;
	float distance = glm::length(glm::vec2(reference.x,reference.y) - glm::vec2(destination.x, destination.y));
	glm::vec3 movement = glm::vec3(reference.x, reference.y, reference.z);

	if (distance > REACH_DESTINATION_ERROR)
	{
		glm::vec3 range = rangeSpeed*normalize(destination - reference);

		movement.x = movement.x + range.x;
		movement.y = movement.y + range.y;

		movDef.direction = getDirectionBasedOnVector(range);
		movDef.doMove = true;

		if (!map->coordinateHasCollision(movement))
			movDef.movement = movement;
		else
			movDef.movement = destination;
	}
	else
		movDef.doMove = false;

	return movDef;
}