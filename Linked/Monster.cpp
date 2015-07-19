#include "Monster.h"
#include "MonsterAI.h"
#include "Display.h"
#include "Map.h"
#include "Time.h"
#include "Player.h"
#include <iostream>

Monster::Monster(Transform* transform, Mesh* mesh, Texture* texture) : Entity(transform, mesh, texture)
{
	this->ai = new MonsterAI();
	setName(MONSTER_DEFAULT_NAME);
	setHp(MONSTER_DEFAULT_HP);
	setTotalMaximumHp(MONSTER_DEFAULT_TOTAL_MAX_HP);
	setTotalAttack(MONSTER_DEFAULT_TOTAL_ATTACK);
	setTotalDefense(MONSTER_DEFAULT_TOTAL_DEFENSE);
	setTotalMagicalPower(MONSTER_DEFAULT_TOTAL_MAGICAL_POWER);
	setTotalSpeed(MONSTER_DEFAULT_TOTAL_SPEED);
	setTotalAttackSpeed(MONSTER_DEFAULT_TOTAL_ATTACK_SPEED);
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

bool Monster::isDead()
{
	return getHp() == 0;
}

unsigned int Monster::getHp()
{
	return hp;
}

void Monster::setHp(unsigned int hp)
{
	this->hp = hp;
}

unsigned int Monster::getTotalMaximumHp()
{
	return totalMaximumHp;
}

void Monster::setTotalMaximumHp(unsigned int totalMaximumHp)
{
	this->totalMaximumHp = totalMaximumHp;
}

void Monster::doDamage(unsigned int damage)
{
	if (damage > hp)
		hp = 0;
	else
		hp = hp - damage;
}

unsigned int Monster::getTotalAttack()
{
	return totalAttack;
}

void Monster::setTotalAttack(unsigned int totalAttack)
{
	this->totalAttack = totalAttack;
}

unsigned int Monster::getTotalDefense()
{
	return totalDefense;
}

void Monster::setTotalDefense(unsigned int totalDefense)
{
	this->totalDefense = totalDefense;
}

unsigned int Monster::getTotalMagicalPower()
{
	return totalMagicalPower;
}

void Monster::setTotalMagicalPower(unsigned int totalMagicalPower)
{
	this->totalMagicalPower = totalMagicalPower;
}

unsigned int Monster::getTotalSpeed()
{
	return totalSpeed;
}

void Monster::setTotalSpeed(unsigned int totalSpeed)
{
	this->totalSpeed = totalSpeed;
}

unsigned int Monster::getTotalRange()
{
	return totalRange;
}

void Monster::setTotalRange(unsigned int totalRange)
{
	this->ai->setMonsterRange(totalRange);
	this->totalRange = totalRange;
}

unsigned int Monster::getTotalCollisionRange()
{
	return this->totalCollisionRange;
}

void Monster::setTotalCollisionRange(unsigned int totalCollisionRange)
{
	this->totalCollisionRange = totalCollisionRange;
}

unsigned int Monster::getTotalAttackSpeed()
{
	return this->totalAttackSpeed;
}

void Monster::setTotalAttackSpeed(unsigned int totalAttackSpeed)
{
	this->totalAttackSpeed = totalAttackSpeed;
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
	float rangeSpeed = getTotalSpeed() * (float)Display::frameTime;
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
	float rangeSpeed = getTotalSpeed() * (float)Display::frameTime;

	MovementDefinition movement = ai->movePerfectlyAway(map, this->getTransform()->getPosition(),
		entity->getTransform()->getPosition(), rangeSpeed);

	if (movement.doMove)
		this->getTransform()->translate(movement.movement.x, movement.movement.y, movement.movement.z);
}

void Monster::moveRandomly(Map* map)
{
	float rangeSpeed = getTotalSpeed() * (float)Display::frameTime;

	if (!ai->isMovingRandomly())
		ai->startRandomMovement(map, this->getTransform()->getPosition(), rangeSpeed);

	MovementDefinition movement = ai->nextRandomStep();

	if (movement.doMove)
		this->getTransform()->translate(movement.movement.x, movement.movement.y, movement.movement.z);
}

bool Monster::hasReachedEntity(Entity* entity)
{
	float distance = glm::length(this->getTransform()->getPosition() - entity->getTransform()->getPosition());

	if (distance < (getTotalRange() / RANGE_DIVIDER))
		return true;
	else
		return false;
}

void Monster::attackCreature(Creature* creature)
{
	double now = Time::getTime();
	bool shouldAttack = (now - lastAttackTime) > ((1.0f/getTotalAttackSpeed()) * ASPD_FACTOR);

	if (shouldAttack)
	{
		unsigned int damage = (unsigned int)ceil(getTotalAttack()/(creature->getTotalDefense()/10.f));
		creature->doDamage(damage);
		lastAttackTime = now;

		/* TEMPORARY (FOR TESTS) */
		std::cout << "Player Attacked." << std::endl << "Player Current Hp: " << creature->getHp() << std::endl;
	}
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

void Monster::update(Map* map, Player* player)
{
	if (player->isDead())
	{
		this->ai->stopMovingToPosition();
		this->moveRandomly(map);
	}
	else
	{
		if (this->hasReachedEntity(player))
		{
			this->attackCreature(player);
		}
		else
		{
			this->moveTo(player, map);
		}
	}
}