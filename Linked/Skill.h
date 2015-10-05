#include "Entity.h"
#include "Movement.h"

class Monster;
class Text;

#pragma once

enum SkillSlot{
	SLOT_1,
	SLOT_2,
	SLOT_3,
	SLOT_4
};

class Skill : public Entity
{
public:
	Skill(std::vector<Monster*> *monsters);
	~Skill();
	SkillSlot getSlot();
	void setSlot(SkillSlot slot);
	bool isActive();
	virtual void use(MovementDirection direction) = 0;
	virtual void update() = 0;

	// Creature that "owns" this skill and will use it eventually
	void setEntity(Entity* entity);
	Entity* getEntity();
	virtual Text* getSkillText() = 0;
protected:
	SkillSlot slot;
	bool active;
	Entity* entity;
	std::vector<Monster*> *monsters;
	Text* skillText;
};

