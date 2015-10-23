#include "Skill.h"

Skill::Skill(std::vector<Monster*> *monsters)
{
	slot = SLOT_1;
	active = false;
	this->monsters = monsters;
}

Skill::~Skill()
{
}

SkillSlot Skill::getSlot()
{
	return slot;
}

void Skill::setSlot(SkillSlot slot)
{
	this->slot = slot;
}

bool Skill::isActive()
{
	return active;
}

WorldObject* Skill::getWorldObject()
{
	return this->worldObject;
}

void Skill::setWorldObject(WorldObject* worldObject)
{
	this->worldObject = worldObject;
}