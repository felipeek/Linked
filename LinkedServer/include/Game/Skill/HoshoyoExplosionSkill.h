#include "Skill.h"

#pragma once
class HoshoyoExplosionSkill : public Skill
{
public:
	HoshoyoExplosionSkill(SkillOwner owner, std::vector<Monster*>* monsters, std::vector<Player*>* players);
	~HoshoyoExplosionSkill();

	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	void update();
private:
	void hitEnemiesOnSkillRadius();
	glm::vec3 explosionPosition;
	int skillPhase;
};

