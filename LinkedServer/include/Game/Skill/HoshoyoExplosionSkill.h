#include "Skill.h"

#pragma once
class HoshoyoExplosionSkill : public Skill
{
public:
	HoshoyoExplosionSkill(SkillOwner owner);
	~HoshoyoExplosionSkill();

	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	void update(std::vector<Monster*> *monsters, std::vector<Player*> *players);
private:
	void hitEnemiesOnSkillRadius(std::vector<Monster*> *monsters);
	glm::vec3 explosionPosition;
	int skillPhase;
};

