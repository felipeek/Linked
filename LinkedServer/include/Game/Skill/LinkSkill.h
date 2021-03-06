#pragma once
#include "Skill.h"

class LinkSkill : public Skill
{
public:
	LinkSkill(SkillOwner owner);
	~LinkSkill();
	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	void update(std::vector<Monster*> *monsters, std::vector<Player*> *players);
private:
	bool linked;
};

