#include "AI.h"

#define REACH_DESTINATION_ERROR 0.5f

#pragma once
class PlayerAI : public AI
{
public:
	PlayerAI();
	~PlayerAI();

	MovementDefinition movePerfectlyTo(Map* map, glm::vec3 reference, glm::vec3 destination, float rangeSpeed);
};

