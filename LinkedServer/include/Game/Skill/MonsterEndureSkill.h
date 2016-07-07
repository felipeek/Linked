#pragma once
#include "Skill.h"

#define TIME_OFFSET 0.2f
#define SPEED_BONUS 10

class MonsterEndureSkill : public Skill
{
public:
	MonsterEndureSkill(SkillOwner owner);
	~MonsterEndureSkill();
	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	void update(std::vector<Monster*> *monsters, std::vector<Player*> *players);
private:
	int skillPhase = 0;
	double timeReference = 0;
};

