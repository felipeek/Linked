#pragma once
#include "AI.h"

#define STANDARD_MONSTER_RANGE 50
#define RANGE_DIVIDER 10.0
#define BORDER_MOVEMENT_LIMIT 2.0
#define POSITION_KEEP_MOVING_FACTOR 100
#define LIMIT_DISTANCE 25

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
	Map* positionMovementMap;
	glm::vec3 positionMovementReference;
	MovementDirection positionMovementDirection;
	glm::vec3 positionMovementMoveRange;
	glm::vec3 positionVirtualTravelledDistance;
	float positionMovementRangeSpeed;
	bool movingToPosition;

	unsigned int monsterRange;
};

