#pragma once
#include "Skill.h"

class MonsterEndureSkill : public Skill
{
public:
	MonsterEndureSkill(SkillOwner owner);
	~MonsterEndureSkill();
	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	void update(std::vector<Monster*> *monsters, std::vector<Player*> *players);
private:
};

