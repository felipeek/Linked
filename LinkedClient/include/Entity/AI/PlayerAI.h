#pragma once
#include "Player.h"
#include "AI.h"

class PlayerAI : public AI
{
public:
	PlayerAI(Player& aiOwner);
	~PlayerAI();
	glm::vec3 getNextStep(glm::vec3 destination) const;
};

