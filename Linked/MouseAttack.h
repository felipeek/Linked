#pragma once
#include <glm\glm.hpp>

#define VECTORSIZE 10
#define COOLDOWN 0

class MouseAttack
{
public:
	MouseAttack();
	~MouseAttack();
	void setAttackPos(float x, float y);
	glm::vec3 attack(glm::vec3 playerPosition);
private:
	glm::vec2 screenPos;
	double lastTime;
};

