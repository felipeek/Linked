#pragma once
#include "Skill.h"

class MonsterExplosionSkill : public Skill
{
public:
	MonsterExplosionSkill(SkillOwner owner);
	~MonsterExplosionSkill();
	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	void update(std::vector<Monster*> *monsters, std::vector<Player*> *players);
private:
	int skillPhase = 0;
};

