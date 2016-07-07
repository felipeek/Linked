#pragma once
#include "Skill.h"

#define CURE_BLESSING_SKILL_COOLDOWN 15
#define CURE_BLESSING_SKILL_PERCENTAGE 30

enum class CureBlessingSkillStatus
{
	IDLE,
	EXECUTION
};

class CureBlessingSkill : public Skill
{
public:
	CureBlessingSkill(SkillOwner owner);
	~CureBlessingSkill();
	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	void update(std::vector<Monster*> *monsters, std::vector<Player*> *players);
private:
	CureBlessingSkillStatus status;
	bool checkCooldown = false;
};

