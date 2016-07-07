#pragma once
#include "Skill.h"

enum class SwapSkillStatus
{
	ANIMATION,
	EXECUTION
};

class SwapSkill : public Skill
{
public:
	SwapSkill(SkillOwner owner);
	~SwapSkill();
	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	bool cancelIfPossible();
	void update(std::vector<Monster*> *monsters, std::vector<Player*> *players);
private:
	SwapSkillStatus status;
};