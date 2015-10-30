#include "Skill.h"
#include "Entity.h"
#include "SkillIcon.h"
#include "Player.h"
#include "LinkedTime.h"

Skill::Skill(SkillOwner owner, std::vector<Monster*> *monsters, std::vector<Player*> *players, Player** localPlayer) : Entity(nullptr, nullptr, nullptr)
{
	slot = SLOT_1;
	active = false;
	entity = nullptr;
	this->owner = owner;
	this->monsters = monsters;
	this->players = players;
	this->localPlayer = localPlayer;
}

Skill::~Skill()
{
	if (skillIcon != nullptr)
		delete skillIcon;
}

SkillSlot Skill::getSlot()
{
	return slot;
}

void Skill::setSlot(SkillSlot slot)
{
	this->slot = slot;
	if (skillIcon != NULL) skillIcon->setSlot(slot);
}

bool Skill::isActive()
{
	return active;
}

Entity* Skill::getEntity()
{
	return this->entity;
}

void Skill::setSkillOwner(SkillOwner owner)
{
	this->owner = owner;
}

SkillOwner Skill::getSkillOwner() const
{
	return this->owner;
}

void Skill::setEntity(Entity* entity)
{
	this->entity = entity;
}

SkillIcon* Skill::getSkillIcon()
{
	return this->skillIcon;
}

int Skill::getCooldown() const
{
	return this->cooldown;
}

bool Skill::isOnCooldown() const
{
	double now = LinkedTime::getTime();
	double delta = now - cooldownContage;
	if (delta < (double)cooldown)
		return true;
	else
		return false;
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