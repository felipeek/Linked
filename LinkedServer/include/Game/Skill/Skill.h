#include "Movement.h"
#include "WorldObject.h"
#include <vector>

class Monster;

#pragma once

enum SkillSlot {
	SLOT_1 = 1,
	SLOT_2 = 2,
	SLOT_3 = 3,
	SLOT_4 = 4
};

class Skill : public WorldObject
{
public:
	Skill(std::vector<Monster*> *monsters);
	~Skill();
	SkillSlot getSlot();
	void setSlot(SkillSlot slot);
	bool isActive();
	virtual void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId) = 0;
	virtual void update() = 0;

	// Creature that "owns" this skill and will use it eventually
	void setWorldObject(WorldObject* entity);
	WorldObject* getWorldObject();
protected:
	SkillSlot slot;
	bool active;
	WorldObject* worldObject;
	std::vector<Monster*> *monsters;
};

