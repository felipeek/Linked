#include "Monster.h"
#include "MonsterAI.h"
#include "Display.h"
#include "Map.h"

Monster::Monster(Transform* transform, Mesh* mesh, Texture* texture) : Entity(transform, mesh, texture)
{
	this->ai = new MonsterAI();
}

Monster::~Monster()
{
	delete this->ai;
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
	return this->hp;
}

void Monster::setHp(unsigned int hp)
{
	this->hp = hp;
}

unsigned int Monster::getMaxHp()
{
	return maxHp;
}

void Monster::setMaxHp(unsigned int maxHp)
{
	this->maxHp = maxHp;
}

void Monster::doDamage(unsigned int damage)
{
	if (damage > hp)
		hp = 0;
	else
		hp = hp - damage;
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

unsigned int Monster::getSpeed()
{
	return speed;
}

void Monster::setSpeed(unsigned int speed)
{
	this->speed = speed;
}

unsigned int Monster::getRange()
{
	return range;
}

void Monster::setRange(unsigned int range)
{
	this->ai->setMonsterRange(range);
	this->range = range;
}

unsigned int Monster::getCollisionRange()
{
	return this->collisionRange;
}

void Monster::setCollisionRange(unsigned int collisionRange)
{
	this->collisionRange = collisionRange;
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

void Monster::moveTo(Entity* entity, Map* map)
{
	float rangeSpeed = speed * (float)Display::frameTime;
	MovementDefinition movement;

	movement = ai->moveToDestination(map, this->getTransform()->getPosition(), entity->getTransform()->getPosition(), rangeSpeed);

	if (movement.doMove)
	{
		this->getTransform()->translate(movement.movement.x, movement.movement.y, movement.movement.z);
		changeTextureBasedOnMovementDirection(movement.direction);
	}
}

void Monster::moveAway(Entity* entity, Map* map)
{
	float rangeSpeed = speed * (float)Display::frameTime;

	MovementDefinition movement = ai->movePerfectlyAway(map, this->getTransform()->getPosition(),
		entity->getTransform()->getPosition(), rangeSpeed);

	if (movement.doMove)
		this->getTransform()->translate(movement.movement.x, movement.movement.y, movement.movement.z);
}

void Monster::changeTextureBasedOnMovementDirection(MovementDirection direction)
{
	switch (direction)
	{
	case TOP:
	case TOP_LEFT:
		this->getTexture()->setIndex(3);
		break;
	case RIGHT:
	case TOP_RIGHT:
		this->getTexture()->setIndex(0);
		break;
	case BOTTOM:
	case BOTTOM_RIGHT:
		this->getTexture()->setIndex(2);
		break;
	case LEFT:
	case BOTTOM_LEFT:
		this->getTexture()->setIndex(1);
		break;
	}
}