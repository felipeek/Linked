#include "Monster.h"
#include "MonsterAI.h"
#include "Display.h"
#include "Map.h"
#include "Time.h"
#include "Player.h"
#include <iostream>
#include <vector>

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

	this->alive = true;

	this->currentDirection = MONSTER_FIRST_DIRECTION;
	this->lastIndexTexture = MONSTER_FIRST_INDEX_TEXTURE;
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

bool Monster::isAlive()
{
	return this->alive;
}

bool Monster::isOnScreen()
{
	if (!this->isAlive())
	{
		double now = Time::getTime();
		return (now - killTime) <= DEATH_TIME;
	}
	return true;
}

bool Monster::isAttacking()
{
	if (this->attacking)
	{
		double now = Time::getTime();
		if ((now - lastAttackTime) > ((1.0f / getTotalAttackSpeed()) * ASPD_FACTOR))
		{
			this->attacking = false;
			return false;
		}
		else
			return true;
	}
	return false;
}

bool Monster::isReceivingDamage()
{
	if (this->receivingDamage)
	{
		double now = Time::getTime();
		if ((now - lastReceivedDamageTime) > RECEIVE_DAMAGE_DELAY)
		{
			this->receivingDamage = false;
			return false;
		}
		else
			return true;
	}
	return false;
}

void Monster::killMonster(int index)
{
	this->alive = false;
	this->killTime = Time::getTime();
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

	this->receivingDamage = true;
	lastReceivedDamageTime = Time::getTime();
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

MovementDefinition Monster::moveTo(Entity* entity, Map* map)
{
	float rangeSpeed = getTotalSpeed() * (float)Display::frameTime;

	MovementDefinition movement = ai->moveToDestination(map, this->getTransform()->getPosition(), entity->getTransform()->getPosition(), rangeSpeed);

	if (movement.doMove)
		this->getTransform()->translate(movement.movement.x, movement.movement.y, movement.movement.z);

	return movement;
}

MovementDefinition Monster::moveAway(Entity* entity, Map* map)
{
	float rangeSpeed = getTotalSpeed() * (float)Display::frameTime;
	this->ai->stopMovingRandomly();

	MovementDefinition movement = ai->movePerfectlyAway(map, this->getTransform()->getPosition(),
		entity->getTransform()->getPosition(), rangeSpeed);

	if (movement.doMove)
		this->getTransform()->translate(movement.movement.x, movement.movement.y, movement.movement.z);

	return movement;
}

MovementDefinition Monster::moveRandomly(Map* map)
{
	float rangeSpeed = getTotalSpeed() * (float)Display::frameTime;
	this->ai->stopMovingToPosition();

	if (!ai->isMovingRandomly())
		ai->startRandomMovement(map, this->getTransform()->getPosition(), rangeSpeed);

	MovementDefinition movement = ai->nextRandomStep();

	if (movement.doMove)
		this->getTransform()->translate(movement.movement.x, movement.movement.y, movement.movement.z);

	return movement;
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
	this->attacking = true;
	unsigned int damage = (unsigned int)ceil(getTotalAttack()/(creature->getTotalDefense()/10.f));
	creature->doDamage(damage);
	lastAttackTime = Time::getTime();
}

void Monster::changeTexture(MovementDirection direction)
{
	double now = Time::getTime();
	bool shouldChangeTexture = (now - textureChangeTime) > (TEXTURE_CHANGE_DELAY);

	bool isDead = !this->isAlive();
	bool isAttacking = this->isAttacking();
	bool isReceivingDamage = this->isReceivingDamage();

	if (shouldChangeTexture || isDead != lastIsDead || isAttacking != lastIsAttacking || isReceivingDamage != lastIsReceivingDamage)
	{
		if (isDead)
			changeTextureBasedOnDirection(direction, 48, 48);
		else
		{
			switch (direction)
			{
			case TOP:
			case TOP_LEFT:
			case TOP_RIGHT:
				if (!isAttacking && !isReceivingDamage) changeTextureBasedOnDirection(direction, 0, 3);
				else if (isAttacking) changeTextureBasedOnDirection(direction, 16, 19);
				else if (isReceivingDamage) changeTextureBasedOnDirection(direction, 32, 35);
				break;
			case RIGHT:
			case BOTTOM_RIGHT:
				if (!isAttacking && !isReceivingDamage) changeTextureBasedOnDirection(direction, 8, 11);
				else if (isAttacking) changeTextureBasedOnDirection(direction, 24, 27);
				else if (isReceivingDamage) changeTextureBasedOnDirection(direction, 40, 43);
				break;
			case BOTTOM:
				if (!isAttacking && !isReceivingDamage) changeTextureBasedOnDirection(direction, 12, 15);
				else if (isAttacking) changeTextureBasedOnDirection(direction, 28, 31);
				else if (isReceivingDamage) changeTextureBasedOnDirection(direction, 44, 47);
				break;
			case LEFT:
			case BOTTOM_LEFT:
				if (!isAttacking && !isReceivingDamage) changeTextureBasedOnDirection(direction, 4, 7);
				else if (isAttacking) changeTextureBasedOnDirection(direction, 20, 23);
				else if (isReceivingDamage) changeTextureBasedOnDirection(direction, 36, 39);
				break;
			}
		}

		this->currentDirection = direction;
		this->lastIsAttacking = this->isAttacking();
		this->lastIsReceivingDamage = this->isReceivingDamage();
		this->lastIsDead = !this->isAlive();
		this->textureChangeTime = now;
	}
}

void Monster::changeTextureBasedOnDirection(MovementDirection direction, unsigned int initialTextureIndex, unsigned int finalTextureIndex)
{
	if (direction != currentDirection || this->isAttacking() != lastIsAttacking || this->isReceivingDamage() != lastIsReceivingDamage || !this->isAlive() != lastIsDead)
	{
		this->getTexture()->setIndex(initialTextureIndex);
		this->lastIndexTexture = initialTextureIndex;
	}
	else
	{
		if (this->lastIndexTexture < finalTextureIndex)
		{
			this->getTexture()->setIndex(++this->lastIndexTexture);
		}
		else
		{
			this->getTexture()->setIndex(initialTextureIndex);
			this->lastIndexTexture = initialTextureIndex;
		}
	}
}

void Monster::update(Map* map, Player* player)
{
	MovementDefinition movementDefinition;

	// If the monster is dead, it can't move.
	if (!this->isAlive())
		movementDefinition.doMove = false;
	// If the monster is attacking, it can't move.
	else if (this->isAttacking())
		movementDefinition.doMove = false;
	// If the monster is receiving damage, it can't move.
	else if (this->isReceivingDamage())
		movementDefinition.doMove = false;
	// If the player is dead, the monster will move randomly.
	else if (!player->isAlive())
		movementDefinition = this->moveRandomly(map);
	// If the monster has reached the player, it will attack the player and not move.
	else if (this->hasReachedEntity(player))
	{
		this->attackCreature(player);
		movementDefinition.doMove = false;
	}
	// If the monster has not reached the player yet, it will move towards the player.
	else
		movementDefinition = this->moveTo(player, map);

	// Change monster's texture.
	if (movementDefinition.doMove)
		this->changeTexture(movementDefinition.direction);
	else
		this->changeTexture(currentDirection);
}