#include "Skill.h"

#pragma once
class HoshoyoExplosionSkill : public Skill
{
public:
	HoshoyoExplosionSkill(std::vector<Monster*>* monsters);
	~HoshoyoExplosionSkill();

	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	void update();
private:
	void hitEnemiesOnSkillRadius();
	glm::vec3 explosionPosition;
	int skillPhase;
};

