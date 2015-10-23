#include "Monster.h"
#include "MonsterAI.h"
#include "Map.h"
#include "LinkedTime.h"
#include "Player.h"
#include "PacketController.h"
#include <iostream>
#include <vector>

Monster::Monster()
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
	setTotalRange(MONSTER_DEFAULT_TOTAL_RANGE);

	this->alive = true;
	this->updatedToClient = false;
	this->isKnockedBack = false;
}

bool Monster::wasUpdatedToClient()
{
	return this->updatedToClient;
}

void Monster::setUpdatedToClient(bool updatedToClient)
{
	this->updatedToClient = updatedToClient;
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

bool Monster::isAttacking()
{
	if (this->attacking)
	{
		double now = LinkedTime::getTime();
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
		double now = LinkedTime::getTime();
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

bool Monster::isMoving()
{
	return this->moving;
}

void Monster::attack()
{
	this->attacking = true;
	lastAttackTime = LinkedTime::getTime();
	PacketController::queueMonsterAttack(this->getId());
}

void Monster::receiveDamage()
{
	this->receivingDamage = true;
	lastReceivedDamageTime = LinkedTime::getTime();
	if (rand() % 4) this->isKnockedBack = false;
	else this->isKnockedBack = true;
}

bool Monster::isOnScreen()
{
	if (!this->isAlive())
	{
		double now = LinkedTime::getTime();
		return (now - killTime) <= DEATH_TIME;
	}
	return true;
}

void Monster::killMonster()
{
	this->alive = false;
	this->killTime = LinkedTime::getTime();
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
	if (damage >= hp)
	{
		hp = 0;
		this->killMonster();
	}
	else
		hp = hp - damage;

	this->receiveDamage();
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

MovementDefinition Monster::moveTo(WorldObject* worldObject, Map* map)
{
	float rangeSpeed = getTotalSpeed() * FRAMETIME;

	MovementDefinition movement = ai->moveToDestination(map, this->getPosition(), worldObject->getPosition(), rangeSpeed);

	if (movement.doMove)
		this->setPosition(movement.movement);

	return movement;
}

MovementDefinition Monster::moveRandomly(Map* map)
{
	float rangeSpeed = getTotalSpeed() * FRAMETIME;
	this->ai->stopMovingToPosition();

	if (!ai->isMovingRandomly())
		ai->startRandomMovement(map, this->getPosition(), rangeSpeed);

	MovementDefinition movement = ai->nextRandomStep();

	if (movement.doMove)
		this->setPosition(movement.movement);

	return movement;
}

bool Monster::hasReachedEntity(WorldObject* worldObject)
{
	float distance = glm::length(this->getPosition() - worldObject->getPosition());

	if (distance < (getTotalRange() / RANGE_DIVIDER))
		return true;
	else
		return false;
}

void Monster::attackCreature(Creature* creature)
{
	unsigned int damage = (unsigned int)ceil(getTotalAttack() / (creature->getTotalDefense() / 10.f));
	creature->doDamage(damage);
	this->attack();
}

void Monster::update(Map* map, std::vector<Player*>* players)
{
	Player* targetPlayer = this->getClosestAlivePlayer(players);
	MovementDefinition movementDefinition;

	// If the monster is dead, it can't move.
	if (!this->isAlive())
		movementDefinition.doMove = false;
	// If the monster is attacking, it can't move.
	else if (this->isAttacking())
		movementDefinition.doMove = false;
	// If the monster is receiving damage, it can't move.
	else if (this->isReceivingDamage() && this->isKnockedBack)
		movementDefinition.doMove = false;
	// If the player is dead, the monster will move randomly.
	else if (targetPlayer == NULL || !targetPlayer->isAlive())
		movementDefinition = this->moveRandomly(map);
	// If the monster has reached the player, it will attack the player and not move.
	else if (targetPlayer != NULL && this->hasReachedEntity(targetPlayer))
	{
		this->attackCreature(targetPlayer);
		movementDefinition.doMove = false;
	}
	// If the monster has not reached the player yet, it will move towards the player.
	else
		movementDefinition = this->moveTo(targetPlayer, map);

	// Change monster's texture.
	if (movementDefinition.doMove)
	{
		this->moving = true;
	}
	else
	{
		this->moving = false;
	}
}

Player* Monster::getClosestAlivePlayer(std::vector<Player*>* players)
{
	Player* targetPlayer = NULL;

	int i = 0;

	if (players->size() > 0)
	{
		glm::vec2 mostClosePosition = glm::vec2();
		bool isFirstPlayer = true;

		std::vector<Player*> players2 = *players;

		i++;

		for (Player* player : *players)
		{
			if (player->isAlive())
			{
				if (isFirstPlayer)
				{
					mostClosePosition = glm::vec2(player->getPosition().x, player->getPosition().y);
					isFirstPlayer = false;
					targetPlayer = player;
				}
				else
				{
					glm::vec2 pPos = glm::vec2(player->getPosition().x, player->getPosition().y);
					double currentBestLength = glm::length(glm::vec2(this->getPosition().x, this->getPosition().y) - mostClosePosition);
					double newLength = glm::length(glm::vec2(this->getPosition().x, this->getPosition().y) - pPos);
					if (newLength <= currentBestLength)
					{
						targetPlayer = player;
						mostClosePosition = pPos;
					}
				}
			}
		}
	}

	return targetPlayer;
}