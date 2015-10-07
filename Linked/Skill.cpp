#include "Skill.h"
#include "Entity.h"
#include "Text.h"
#include "SkillIcon.h"

Skill::Skill(std::vector<Monster*> *monsters) : Entity(nullptr, nullptr, nullptr)
{
	slot = SLOT_1;
	active = false;
	entity = nullptr;
	this->monsters = monsters;
}

Skill::~Skill()
{
	if (mesh != nullptr)
		delete mesh;
	if (texture != nullptr)
		delete texture;
	if (skillText != nullptr)
		delete skillText;
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

void Skill::setEntity(Entity* entity)
{
	this->entity = entity;
}

Text* Skill::getSkillText()
{
	return this->skillText;
}

SkillIcon* Skill::getSkillIcon()
{
	return this->skillIcon;
}