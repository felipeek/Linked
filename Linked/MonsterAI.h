#pragma once
#include "AI.h"
#define KEEP_MOVING_FACTOR 100
#define STANDARD_MONSTER_RANGE 50
#define RANGE_DIVIDER 10.0
#define BORDER_MOVEMENT_LIMIT 2.0

class MonsterAI : public AI
{
public:
	MonsterAI();
	MonsterAI(unsigned int monsterRange);
	~MonsterAI();
	MovementDefinition moveToDestination(Map *monsterMovementMap, glm::vec3 monsterMovementReference, glm::vec3 monsterMovementDestination, float rangeSpeed);

	unsigned int getMonsterRange();
	void setMonsterRange(unsigned int monsterRange);
	void startPositionMovement(Map* map, glm::vec3 reference, glm::vec3 destination, float rangeSpeed);
	MovementDefinition nextPositionMovementStep();
	bool isMovingToPosition();
	void stopMovingToPosition();
private:

private:
	const float PI = 3.14159265358979f;

	Map* positionMovementMap;
	glm::vec3 positionMovementReference;
	glm::vec3 positionMovementDestination;
	glm::vec3 virtualTravelledDistance;
	float positionMovementRangeSpeed;
	bool movingToPosition;

	unsigned int monsterRange;
};

