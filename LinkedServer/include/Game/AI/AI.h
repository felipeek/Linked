#pragma once
#include "glm\glm.hpp"
#include "Movement.h"
class Map;

#define STAND_STILL_RANDOM_FACTOR_MINIMUM 6
#define STAND_STILL_RANDOM_FACTOR_MAXIMUM 10
#define RANDOM_KEEP_MOVING_FACTOR 1000

class AI
{
public:
	AI();
	~AI();
	void startRandomMovement(Map* map, glm::vec3 reference, float rangeSpeed);
	MovementDefinition nextRandomStep();
	bool isMovingRandomly();
	void stopMovingRandomly();
protected:
	MovementDirection getDirectionBasedOnVector(glm::vec3 vector);
	bool checkIfMonsterIsStillOnTheSameMapPosition(glm::vec3 currentPosition, glm::vec3 nextPosition);
private:
	const float PI = 3.14159265358979f;
	Map* randomMovementMap;
	glm::vec3 randomMovementReference;
	float randomMovementRangeSpeed;
	int randomStandStillFactor;
	glm::vec3 randomMovementMoveRange;
	glm::vec3 randomVirtualTravelledDistance;
	MovementDirection randomMovementDirection;
	bool movingRandomly;
	double timeRandomMovementStarted = 0;

};

