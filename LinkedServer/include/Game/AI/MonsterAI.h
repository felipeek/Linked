#pragma once
#include "AI.h"
#include <vector>

class Monster;
class Map;
class Player;

#define RANDOM_KEEP_MOVING_FACTOR 10.0f
#define DIRECTED_KEEP_MOVING_FACTOR 1.5f
#define STAND_STILL_TIME 1.5f
#define RANGE_ATTACK_FACTOR 10.0f
#define RANGE_CHASE_TARGET 20.0f
#define AVOID_WALLS_INTELLIGENCE 5

class MonsterAI : public AI
{
public:
	MonsterAI(Monster& aiOwner);
	~MonsterAI();
	MovementDefinition generateRandomMovement(Map* map) const;
	MovementDefinition generateMovementTowardsCoordinate(glm::vec3 destination) const;
	bool isOnRangeToAttack(glm::vec3 worldObjectPosition) const;
	bool isOnRangeToChaseTarget(glm::vec3 worldObjectPosition) const;
	bool isPathFreeOfCollisions(Map* map, glm::vec3 destination) const;
	bool shouldStandStill() const;
	void resetStandStill();
	glm::vec3 getNextStep(glm::vec3 destination) const;
	Player* findClosestWorldObjectToAttack(Map* map, std::vector<Player*>* players);
private:
	double standStillTime = 0;
};

