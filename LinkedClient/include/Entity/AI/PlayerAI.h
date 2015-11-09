#pragma once
#include "Player.h"

class PlayerAI
{
public:
	PlayerAI(Player& aiOwner);
	~PlayerAI();
	glm::vec3 getNextStep(glm::vec3 destination) const;
	MovementDefinition movePerfectlyTo(Map* map, glm::vec3 reference, glm::vec3 destination, float rangeSpeed) const;
	bool reachDestination(glm::vec3 nextPosition, glm::vec3 destination) const;
	MovementDefinition getMovementDefinitionOfDestination(glm::vec3 destination);
private:
	MovementDirection getCompleteDirection(glm::vec3 vector) const;
	float getVectorAngle(glm::vec3 vector) const;
	const float PI = 3.14159265358979f;
	Player& aiOwner;
};

