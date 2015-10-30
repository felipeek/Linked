#pragma once
#include "Skill.h"

class LinkSkill : public Skill
{
public:
	LinkSkill(SkillOwner owner, std::vector<Monster*>* monsters, std::vector<Player*>* players);
	~LinkSkill();
	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	void update();
private:
	bool linked;
};

