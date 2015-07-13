#include "MouseAttack.h"
#include "Input.h"
#include "Time.h"
#include <iostream>

MouseAttack::MouseAttack()
{
	screenPos = glm::vec2(0, 0);
	lastTime = Time::getTime();
}

MouseAttack::~MouseAttack()
{
}

void MouseAttack::setAttackPos(float x, float y)
{
	screenPos.x = x;
	screenPos.y = y;
}

glm::vec3 MouseAttack::attack(glm::vec3 playerPosition)
{
	glm::vec3 attackVector = playerPosition + (glm::vec3(screenPos, 1.0f) * glm::vec3(VECTORSIZE, VECTORSIZE, 1));
	Input::attack = false;
	return attackVector;
}
