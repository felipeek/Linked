#pragma once
#include "LinkedTime.h"
#include "AI.h"

class Monster;
class Map;

#define RANDOM_KEEP_MOVING_FACTOR 10.0f
#define DIRECTED_KEEP_MOVING_FACTOR 1.0f
#define STAND_STILL_TIME 1.5f
#define RANGE_ATTACK_FACTOR 10.0f
#define RANGE_CHASE_TARGET 20.0f
#define AVOID_WALLS_INTELLIGENCE 5

class MonsterAI : public AI
{
public:
	MonsterAI(Monster& aiOwner);
	~MonsterAI();
	glm::vec3 getNextStep(glm::vec3 destination) const;
	MovementDefinition generateRandomMovement(Map* map) const;
	MovementDefinition generateMovementTowardsCoordinate(glm::vec3 destination) const;
	bool isOnRangeToAttack(glm::vec3 worldObjectPosition) const;
	bool isOnRangeToChaseTarget(glm::vec3 worldObjectPosition) const;
	bool isPathFreeOfCollisions(Map* map, glm::vec3 destination) const;
	bool shouldStandStill() const;
	void resetStandStill();
private:
	double standStillTime = 0;
};

