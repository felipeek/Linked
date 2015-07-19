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

	this->currentDirection = TOP_LEFT;	// initial direction (standard value)
	this->alive = true;
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

void Monster::killMonster()
{
	this->alive = false;
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

MovementDirection Monster::moveTo(Entity* entity, Map* map)
{
	float rangeSpeed = getTotalSpeed() * (float)Display::frameTime;

	MovementDefinition movement = ai->moveToDestination(map, this->getTransform()->getPosition(), entity->getTransform()->getPosition(), rangeSpeed);

	if (movement.doMove)
		this->getTransform()->translate(movement.movement.x, movement.movement.y, movement.movement.z);

	return movement.direction;
}

MovementDirection Monster::moveAway(Entity* entity, Map* map)
{
	float rangeSpeed = getTotalSpeed() * (float)Display::frameTime;
	this->ai->stopMovingRandomly();

	MovementDefinition movement = ai->movePerfectlyAway(map, this->getTransform()->getPosition(),
		entity->getTransform()->getPosition(), rangeSpeed);

	if (movement.doMove)
		this->getTransform()->translate(movement.movement.x, movement.movement.y, movement.movement.z);

	return movement.direction;
}

MovementDirection Monster::moveRandomly(Map* map)
{
	float rangeSpeed = getTotalSpeed() * (float)Display::frameTime;
	this->ai->stopMovingToPosition();

	if (!ai->isMovingRandomly())
		ai->startRandomMovement(map, this->getTransform()->getPosition(), rangeSpeed);

	MovementDefinition movement = ai->nextRandomStep();

	if (movement.doMove)
		this->getTransform()->translate(movement.movement.x, movement.movement.y, movement.movement.z);

	return movement.direction;
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

	//this->changeTexture(ATTACKING);
}

void Monster::changeTexture(MovementDirection direction)
{
	double now = Time::getTime();
	bool shouldChangeTexture = (now - textureChangeTime) > (TEXTURE_CHANGE_DELAY);

	if (direction != currentDirection || shouldChangeTexture)
	{
		switch (direction)
		{
			case TOP:
			case TOP_LEFT:
			case TOP_RIGHT:
				if (direction != currentDirection)
				{
					this->getTexture()->setIndex(5);
					this->lastIndexTexture = 5;
				}
				else
				{
					if (this->lastIndexTexture < 8)
						this->getTexture()->setIndex(++this->lastIndexTexture);
					else
					{
						this->getTexture()->setIndex(5);
						this->lastIndexTexture = 5;
					}
				}
				break;
			case RIGHT:
			case BOTTOM_RIGHT:
				if (direction != currentDirection)
				{
					this->getTexture()->setIndex(15);
					this->lastIndexTexture = 15;
				}
				else if (shouldChangeTexture)
				{
					if (this->lastIndexTexture < 18)
						this->getTexture()->setIndex(++this->lastIndexTexture);
					else
					{
						this->getTexture()->setIndex(15);
						this->lastIndexTexture = 15;
					}
				}
				break;
			case BOTTOM:
				if (direction != currentDirection)
				{
					this->getTexture()->setIndex(20);
					this->lastIndexTexture = 20;
				}
				else if (shouldChangeTexture)
				{
					if (this->lastIndexTexture < 23)
						this->getTexture()->setIndex(++this->lastIndexTexture);
					else
					{
						this->getTexture()->setIndex(20);
						this->lastIndexTexture = 20;
					}
				}
				break;
			case LEFT:
			case BOTTOM_LEFT:
				if (direction != currentDirection)
				{
					this->getTexture()->setIndex(10);
					this->lastIndexTexture = 10;
				}
				else if (shouldChangeTexture)
				{
					if (this->lastIndexTexture < 13)
						this->getTexture()->setIndex(++this->lastIndexTexture);
					else
					{
						this->getTexture()->setIndex(10);
						this->lastIndexTexture = 10;
					}
				}
				break;
			case ATTACKING:
				break;
			case RECEIVING_DAMAGE:
				break;
			case DEAD:
				this->getTexture()->setIndex(0);
				this->lastIndexTexture = 0;
				break;
		}

		this->currentDirection = direction;
		this->textureChangeTime = now;
	}
}

void Monster::update(Map* map, Player* player)
{
	MovementDirection monsterDirection;

	if (!this->isAlive()){
		//monsterDirection = DEAD;
	}
	else if (!player->isAlive())
		monsterDirection = this->moveRandomly(map);
	else if (this->hasReachedEntity(player))
	{
		this->attackCreature(player);
		//monsterDirection = ATTACKING;
	}	
	else
		monsterDirection = this->moveTo(player, map);

	this->changeTexture(monsterDirection);
}