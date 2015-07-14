#include "Monster.h"

Monster::Monster(Transform* transform, Mesh* mesh, Texture* texture) : Entity(transform, mesh, texture)
{
}

Monster::~Monster()
{
}

std::string Monster::getName()
{
	return name;
}

void Monster::setName(std::string name)
{
	this->name = name;
}

unsigned int Monster::getHp()
{
	return hp;
}

void Monster::setHp(unsigned int hp)
{
	this->hp = hp;
}

unsigned int Monster::getAttack()
{
	return attack;
}

void Monster::setAttack(unsigned int attack)
{
	this->attack = attack;
}

unsigned int Monster::getDefense()
{
	return defense;
}

void Monster::setDefense(unsigned int defense)
{
	this->defense = defense;
}

glm::vec3 Monster::getMapColor()
{
	return mapColor;
}

void Monster::setMapColor(glm::vec3 mapColor)
{
	this->mapColor = mapColor;
}

int Monster::getMapColorRed()
{
	return (int)this->mapColor.r;
}

void Monster::setMapColorRed(int red)
{
	this->mapColor.r = (float)red;
}

int Monster::getMapColorGreen()
{
	return (int)this->mapColor.g;
}

void Monster::setMapColorGreen(int green)
{
	this->mapColor.g = (float)green;
}

int Monster::getMapColorBlue()
{
	return (int)this->mapColor.b;
}

void Monster::setMapColorBlue(int blue)
{
	this->mapColor.b = (float)blue;
}