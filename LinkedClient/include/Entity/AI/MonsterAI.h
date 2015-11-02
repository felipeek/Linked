#pragma once
#include "glm\glm.hpp"
#include "Movement.h"
#include "LinkedTime.h"

class Monster;
class Map;

#define RANDOM_KEEP_MOVING_FACTOR 10.0f
#define DIRECTED_KEEP_MOVING_FACTOR 1.0f
#define STAND_STILL_TIME 1.0f
#define RANGE_ATTACK_FACTOR 10.0f
#define RANGE_CHASE_TARGET 20.0f
#define AVOID_WALLS_INTELLIGENCE 5

class MonsterAI
{
public:
	MonsterAI(Monster& aiOwner);
	~MonsterAI();
	MovementDefinition generateRandomMovement(Map* map) const;
	MovementDefinition generateMovementTowardsCoordinate(glm::vec3 destination) const;
	bool isOnRangeToAttack(glm::vec3 worldObjectPosition) const;
	bool isOnRangeToChaseTarget(glm::vec3 worldObjectPosition) const;
	bool isPathFreeOfCollisions(Map* map, glm::vec3 destination) const;
	bool reachDestination(glm::vec3 nextPosition, glm::vec3 destination) const;
	bool shouldStandStill() const;
	void resetStandStill();
	glm::vec3 getNextStep(glm::vec3 destination) const;
protected:
	MovementDirection getDiagonalDirection(glm::vec3 vector) const;
	MovementDirection getCompleteDirection(glm::vec3 vector) const;
private:
	Monster& aiOwner;
	float getVectorAngle(glm::vec3 vector) const;
	double standStillTime = 0;
	const float PI = 3.14159265358979f;
};

