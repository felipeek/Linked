#pragma once

#include "glm\glm.hpp"

enum MovementDirection {
	TOP = 0,
	LEFT = 1,
	RIGHT = 2,
	BOTTOM = 3,
	TOP_LEFT = 4,
	TOP_RIGHT = 5,
	BOTTOM_LEFT = 6,
	BOTTOM_RIGHT = 7
};

class MovementDefinition
{
public:
	glm::vec3 movement;
	MovementDirection direction;
	bool doMove;
};