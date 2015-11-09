#include "WorldObject.h"

unsigned short WorldObject::NEXT_ID = FIRST_ID;

WorldObject::WorldObject()
{
	this->position = glm::vec3(0, 0, 0);
	this->id = NEXT_ID++;
}

WorldObject::~WorldObject()
{
}

glm::vec3 WorldObject::getPosition()
{
	return this->position;
}

void WorldObject::setPosition(glm::vec3 position)
{
	this->position = position;
}

unsigned short WorldObject::getId()
{
	return this->id;
}

void WorldObject::setId(unsigned short id)
{
	this->id = id;
}