#include "Skill.h"
#include "LinkedTime.h"

Skill::Skill(SkillOwner owner)
{
	slot = SLOT_1;
	active = false;
	this->owner = owner;
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

void Skill::setSkillOwner(SkillOwner owner)
{
	this->owner = owner;
}

SkillOwner Skill::getSkillOwner() const
{
	return this->owner;
}

int Skill::getCooldown() const
{
	return this->cooldown;
}

bool Skill::isOnCooldown() const
{
	if ((LinkedTime::getTime() - cooldownContage) < (double)cooldown)
		return false;
	else
		return true;
}

/* PRIVATE */

void Skill::startCooldownContage()
{
	this->cooldownContage = LinkedTime::getTime();
}

void Skill::resetCooldownContageForcibly()
{
	this->cooldownContage = 0;
}