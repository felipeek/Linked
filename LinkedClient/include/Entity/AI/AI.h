#pragma once
#include "glm\glm.hpp"
#include "Movement.h"

class Entity;

class AI
{
public:
	AI(Entity& aiOwner);
	~AI();
	Entity& getAiOwner() const;
	bool reachDestination(glm::vec3 nextPosition, glm::vec3 destination) const;
	MovementDefinition getMovementDefinitionOfDestination(glm::vec3 destination, bool diagonalDirection);
	MovementDirection getDiagonalDirection(glm::vec3 vector) const;
protected:
	MovementDirection getCompleteDirection(glm::vec3 vector) const;
private:
	float getVectorAngle(glm::vec3 vector) const;
	const float PI = 3.14159265358979f;
	Entity& aiOwner;
};

