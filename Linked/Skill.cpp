#include "Skill.h"
#include "Entity.h"

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
	// TO DO
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