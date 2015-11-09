#pragma once
#include "Player.h"
#include "AI.h"

class PlayerAI : public AI
{
public:
	PlayerAI(Player& aiOwner);
	~PlayerAI();
	MovementDefinition movePerfectlyTo(Map* map, glm::vec3 reference, glm::vec3 destination, float rangeSpeed) const;
private:
	Player& aiOwner;
};

