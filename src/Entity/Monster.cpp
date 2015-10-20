#include "Monster.h"
#include "MonsterAI.h"
#include "Display.h"
#include "Map.h"
#include "LinkedTime.h"
#include "Player.h"
#include "Primitive.h"
#include "Game.h"
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
	setTotalRange(MONSTER_DEFAULT_TOTAL_RANGE);

	this->alive = true;

	this->currentDirection = MONSTER_FIRST_DIRECTION;
	this->lastIndexTexture = MONSTER_FIRST_INDEX_TEXTURE;
	this->isMovingTo = false;
	this->isKnockedBack = false;
}

Monster::~Monster()
{
	delete this->ai;
}

/* *********************************** */
/* ********** PUBLIC METHODS ********* */
/* *********************************** */

/* BASIC ATTRIBUTES */

int Monster::getId()
{
	return this->id;
}

void Monster::setId(int id)
{
	this->id = id;
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

unsigned int Monster::getTotalMaximumHp()
{
	return totalMaximumHp;
}

void Monster::setTotalMaximumHp(unsigned int totalMaximumHp)
{
	this->totalMaximumHp = totalMaximumHp;
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

unsigned int Monster::getTotalMagicalPower()
{
	return totalMagicalPower;
}

void Monster::setTotalMagicalPower(unsigned int totalMagicalPower)
{
	this->totalMagicalPower = totalMagicalPower;
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

glm::vec3 Monster::getMapColor()
{
	return mapColor;
}

void Monster::setMapColor(glm::vec3 mapColor)
{
	this->mapColor = mapColor;
}

/* STATUS */

bool Monster::isAlive()
{
	return this->alive;
}

void Monster::killMonster()
{
	this->alive = false;
	this->killTime = Time::getTime();
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

void Monster::attack()
{
	this->attacking = true;
	lastAttackTime = Time::getTime();
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

void Monster::receiveDamage()
{
	this->receivingDamage = true;
	lastReceivedDamageTime = Time::getTime();
	if (rand() % 2) this->isKnockedBack = false;
	else this->isKnockedBack = true;
}

bool Monster::shouldRender()
{
	return this->bRender;
}

void Monster::setShouldRender(bool bRender)
{
	this->bRender = bRender;
}

bool Monster::isMoving()
{
	return this->moving;
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

/* COMBAT */

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

void Monster::attackCreature(Creature* creature)
{
	unsigned int damage = (unsigned int)ceil(getTotalAttack() / (creature->getTotalDefense() / 10.f));
	creature->doDamage(damage);
	this->attack();
}

/* MOVEMENT */

void Monster::startMovementTo(glm::vec3 destination)
{
	this->destination = destination;
	this->isMovingTo = true;
}

/* UPDATE */

void Monster::update(Map* map, Player* player)
{
	MovementDefinition movementDefinition;

	if (Game::multiplayer)
	{
		movementDefinition = this->updateMovement(map);
	}
	else
	{
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
	}

	// Change monster's texture.
	if (movementDefinition.doMove)
	{
		this->moving = true;
		this->changeTexture(movementDefinition.direction);
	}
	else
	{
		this->moving = false;
		this->changeTexture(currentDirection);
	}
}

/* *********************************** */
/* ********* PRIVATE METHODS ********* */
/* *********************************** */

/* TEXTURE MANAGEMENT AUXILIAR FUNCTIONS */

void Monster::changeTexture(MovementDirection direction)
{
	double now = Time::getTime();
	bool shouldChangeTexture = (now - textureChangeTime) > (TEXTURE_CHANGE_DELAY);

	bool isDead = !this->isAlive();
	bool isAttacking = this->isAttacking();
	bool isReceivingDamage = this->isReceivingDamage();
	bool isMoving = this->isMoving();

	if (shouldChangeTexture || direction != this->currentDirection || isDead != lastIsDead || isAttacking != lastIsAttacking || isReceivingDamage != lastIsReceivingDamage || isMoving != lastIsMoving)
	{
		if (isDead)
			changeTextureBasedOnDirection(direction, 0, 0);
		else
		{
			switch (direction)
			{
			case TOP:
			case TOP_LEFT:
				if (!isAttacking && !isReceivingDamage && isMoving) changeTextureBasedOnDirection(direction, 31, 33);
				else if (isAttacking) changeTextureBasedOnDirection(direction, 7, 9);
				else if (isReceivingDamage) changeTextureBasedOnDirection(direction, 19, 21);
				else if (!isMoving) changeTextureBasedOnDirection(direction, 43, 45);
				break;
			case RIGHT:
			case BOTTOM:
			case BOTTOM_RIGHT:
				if (!isAttacking && !isReceivingDamage && isMoving) changeTextureBasedOnDirection(direction, 28, 30);
				else if (isAttacking) changeTextureBasedOnDirection(direction, 4, 6);
				else if (isReceivingDamage) changeTextureBasedOnDirection(direction, 16, 18);
				else if (!isMoving) changeTextureBasedOnDirection(direction, 40, 42);
				break;
			case TOP_RIGHT:
				if (!isAttacking && !isReceivingDamage && isMoving) changeTextureBasedOnDirection(direction, 34, 36);
				else if (isAttacking) changeTextureBasedOnDirection(direction, 10, 12);
				else if (isReceivingDamage) changeTextureBasedOnDirection(direction, 22, 24);
				else if (!isMoving) changeTextureBasedOnDirection(direction, 46, 48);
				break;
			case LEFT:
			case BOTTOM_LEFT:
				if (!isAttacking && !isReceivingDamage && isMoving) changeTextureBasedOnDirection(direction, 25, 27);
				else if (isAttacking) changeTextureBasedOnDirection(direction, 1, 3);
				else if (isReceivingDamage) changeTextureBasedOnDirection(direction, 13, 15);
				else if (!isMoving) changeTextureBasedOnDirection(direction, 37, 39);
				break;
			}
		}

		this->currentDirection = direction;
		this->lastIsAttacking = this->isAttacking();
		this->lastIsReceivingDamage = this->isReceivingDamage();
		this->lastIsDead = !this->isAlive();
		this->lastIsMoving = this->isMoving();
		this->textureChangeTime = now;
	}
}

void Monster::changeTextureBasedOnDirection(MovementDirection direction, unsigned int initialTextureIndex, unsigned int finalTextureIndex)
{
	if (direction != currentDirection || this->isAttacking() != lastIsAttacking || this->isReceivingDamage() != lastIsReceivingDamage || !this->isAlive() != lastIsDead || this->isMoving() != lastIsMoving)
	{
		this->getMesh()->getQuad()->setIndex(this->decodeMonsterIndex(initialTextureIndex));
		this->lastIndexTexture = initialTextureIndex;
	}
	else
	{
		if (this->lastIndexTexture < finalTextureIndex)
		{
			this->lastIndexTexture++;
			this->getMesh()->getQuad()->setIndex(this->decodeMonsterIndex(this->lastIndexTexture));
		}
		else
		{
			this->getMesh()->getQuad()->setIndex(this->decodeMonsterIndex(initialTextureIndex));
			this->lastIndexTexture = initialTextureIndex;
		}
	}
}

// takes logical index and transforms in real index
// this is needed because the sprite architeture was bad planned (my bad)
int Monster::decodeMonsterIndex(int index)
{
	switch (index)
	{
	case 0: return 6;
	case 1: return 5;
	case 2: return 4;
	case 3: return 3;
	case 4: return 2;
	case 5: return 1;
	case 6: return 0;
	case 7: return 13;
	case 8: return 12;
	case 9: return 11;
	case 10: return 10;
	case 11: return 9;
	case 12: return 8;
	case 13: return 7;
	case 14: return 20;
	case 15: return 19;
	case 16: return 18;
	case 17: return 17;
	case 18: return 16;
	case 19: return 15;
	case 20: return 14;
	case 21: return 27;
	case 22: return 26;
	case 23: return 25;
	case 24: return 24;
	case 25: return 23;
	case 26: return 22;
	case 27: return 21;
	case 28: return 34;
	case 29: return 33;
	case 30: return 32;
	case 31: return 31;
	case 32: return 30;
	case 33: return 29;
	case 34: return 28;
	case 35: return 41;
	case 36: return 40;
	case 37: return 39;
	case 38: return 38;
	case 39: return 37;
	case 40: return 36;
	case 41: return 35;
	case 42: return 48;
	case 43: return 47;
	case 44: return 46;
	case 45: return 45;
	case 46: return 44;
	case 47: return 43;
	case 48: return 42;
	default: return 0;
	}
}

/* MOVEMENT AUXILIAR FUNCTIONS */

MovementDefinition Monster::moveTo(Entity* entity, Map* map)
{
	float rangeSpeed = getTotalSpeed() * (float)Display::frameTime;

	MovementDefinition movement = ai->moveToDestination(map, this->getTransform()->getPosition(), entity->getTransform()->getPosition(), rangeSpeed);

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

MovementDefinition Monster::updateMovement(Map* map)
{
	MovementDefinition newPos = MovementDefinition();
	newPos.doMove = false;

	if (isMovingTo)
	{
		glm::vec3 pPos = this->getTransform()->getPosition();
		float frameTime = (float)Display::frameTime;
		float range = frameTime * this->getTotalSpeed();
		newPos = this->ai->movePerfectlyTo(map, pPos, this->destination, range);

		if (newPos.doMove)
		{
			this->getTransform()->translate(newPos.movement.x, newPos.movement.y, newPos.movement.z);
			this->currentDirection = newPos.direction;
		}
		else
		{
			this->isMovingTo = false;
		}
	}

	return newPos;
}