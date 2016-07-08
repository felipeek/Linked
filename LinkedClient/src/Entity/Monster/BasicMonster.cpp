#include "BasicMonster.h"
#include "Player.h"
#include "MonsterAI.h"
#include "Primitive.h"
#include "Map.h"
#include "Game.h"

BasicMonster::BasicMonster(Transform* transform, Mesh* mesh, Texture* texture) : Monster(transform, mesh, texture)
{
	this->directedMovement.direction = BOTTOM_LEFT;
	this->lastTimeTextureWasRefreshed = 0;
}

BasicMonster::~BasicMonster()
{
}

void BasicMonster::update(Map* map, Player* player, std::vector<Monster*>* monsters)
{
	Monster::update(map, player, monsters);
	this->updateMovement(map, player);
	this->refreshTextureIfNecessary();
}

void BasicMonster::updateMovement(Map* map, Player* player)
{
	double now = LinkedTime::getTime();
	if (this->isAlive())
	{
		if (Game::multiplayer)
			this->moveOnline(map);
		else
		{
			if ((this->ai->isOnRangeToAttack(player->getTransform()->getPosition()) && player->isAlive()))
			{
				if (!this->isAttacking()) this->attackCreature(player);
				this->stop(); // For Texture Management
				this->forceMonsterToLookAtPlayer(player->getTransform()->getPosition());
			}
			else if (this->isAttacking())
			{
				this->stop();
				this->forceMonsterToLookAtPlayer(player->getTransform()->getPosition());
			}
			else if (this->isReceivingDamage() && this->isKnockbackable())
			{
				this->stop();
			}
			else if (this->ai->isOnRangeToChaseTarget(player->getTransform()->getPosition()) && this->ai->isPathFreeOfCollisions(map, player->getTransform()->getPosition()) && player->isAlive())
			{
				this->moveToAttackPlayer(map, player);
				this->movingRandomly = false;
				this->move(this->directedMovement.direction); // For Texture Management
			}
			else if (!this->ai->shouldStandStill())
			{
				this->moveRandomly(map);
				this->movingToAttackPlayer = false;
				this->move(this->directedMovement.direction); // For Texture Management
			}
			else
			{
				this->stop(); // For Texture Management
			}
		}
	}
}

void BasicMonster::render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer)
{
	Monster::render(primitiveShader, skillShader, textRenderer);
}

void BasicMonster::startOnlineMovement(glm::vec3 position)
{
	this->movingOnline = true;
	this->directedMovement = this->ai->getMovementDefinitionOfDestination(position, true);
}

void BasicMonster::action(int actionId, int xid, glm::vec3 vector)
{
}

Monster* BasicMonster::getCopy(Monster* copy)
{
	// Create new monster if necessary.
	if (copy == nullptr) copy = new BasicMonster(nullptr, nullptr, nullptr);

	return Monster::getCopy(copy);
}

void BasicMonster::moveToAttackPlayer(Map* map, Player* player)
{
	glm::vec3 newMonsterPosition = this->ai->getNextStep(this->directedMovement.movement);

	// If the monster is not moving towards the player, starts a new movement towards the player.
	if (!this->movingToAttackPlayer)
	{
		this->directedMovement = this->ai->generateMovementTowardsCoordinate(player->getTransform()->getPosition());
		this->movingToAttackPlayer = true;
	}
	// If the monster was already moving towards the player and reached the destination, it stops moving towards the player.
	if (this->ai->reachDestination(newMonsterPosition, this->directedMovement.movement) && this->movingToAttackPlayer)
	{
		this->movingToAttackPlayer = false;
	}
	// If the monster was already moving towards the player and shouldn't stop, it continues moving towards the player.
	else
	{
		if (!map->coordinateHasCollision(newMonsterPosition))
			this->getTransform()->translate(newMonsterPosition.x, newMonsterPosition.y, this->getTransform()->getPosition().z);
		else
			this->movingToAttackPlayer = false;
	}
}

void BasicMonster::moveRandomly(Map* map)
{
	glm::vec3 newMonsterPosition = this->ai->getNextStep(this->directedMovement.movement);

	// If the monster is not moving randomly, starts a new random movment.
	if (!this->movingRandomly)
	{
		this->directedMovement = this->ai->generateRandomMovement(map);
		this->movingRandomly = true;
	}
	// If the monster was already moving randomly and reached the destintation, it stops moving randomly and prepare to stand still.
	else if (this->ai->reachDestination(newMonsterPosition, this->directedMovement.movement) && this->movingRandomly)
	{
		this->movingRandomly = false;
		this->ai->resetStandStill();
	}
	// If the monster was already moving randomly and shouldn't stop, it continues moving randomly.
	else
	{
		if (!map->coordinateHasCollision(newMonsterPosition))
			this->getTransform()->translate(newMonsterPosition.x, newMonsterPosition.y, this->getTransform()->getPosition().z);
		else
		{
			this->ai->resetStandStill();
			this->movingRandomly = false;
		}
	}
}

void BasicMonster::moveOnline(Map* map)
{
	if (this->movingOnline)
	{
		glm::vec3 newMonsterPosition = this->ai->getNextStep(this->directedMovement.movement);

		if (this->ai->reachDestination(newMonsterPosition, this->directedMovement.movement))
		{
			this->movingOnline = false;
			if (!this->isAttacking() && !this->isReceivingDamage()) timeStopped++;
		}
		else if (!map->coordinateHasCollision(newMonsterPosition))
		{
			this->getTransform()->translate(newMonsterPosition.x, newMonsterPosition.y, this->getTransform()->getPosition().z);
			this->move(this->directedMovement.direction);
			timeStopped = 0;
		}
		else
		{
			this->movingOnline = false;
			if (!this->isAttacking() && !this->isReceivingDamage()) timeStopped++;
		}
	}
	else
		if (!this->isAttacking() && !this->isReceivingDamage()) timeStopped++;

	// In online mode, we can't just change to the STANDING texture when the monster stops walking.
	// This happens because sometimes there is a delay with regards to the packets sent by the server.
	// This delay makes the monster stop for a few milliseconds, which would force a texture change.
	// When this happens, the monster "flicks".
	// The line of code below will make the STANDING texture appears only when the monster has stopped walking for a
	// considerable amount of time.
	if (timeStopped > 5)
		this->stop();
}

void BasicMonster::refreshTextureIfNecessary()
{
	double now = LinkedTime::getTime();

	if (!this->isAlive())
	{
		this->changeTextureIndex(0);
	}
	else if (this->isAttacking())
	{
		bool textureChanged = this->activeTexture != BasicMonsterActiveTexture::ATTACKING || this->activeTextureDirection != this->directedMovement.direction;
		if (textureChanged || (now - this->lastTimeTextureWasRefreshed) > TEXTURE_CHANGE_TIME_FACTOR)
		{
			this->animateActiveTexture();
			this->activeTexture = BasicMonsterActiveTexture::ATTACKING;
			this->activeTextureDirection = this->directedMovement.direction;
			this->lastTimeTextureWasRefreshed = now;
		}
	}
	else if (this->isReceivingDamage())
	{
		bool textureChanged = this->activeTexture != BasicMonsterActiveTexture::RECEIVING_DAMAGE || this->activeTextureDirection != this->directedMovement.direction;
		if (textureChanged || (now - this->lastTimeTextureWasRefreshed) > TEXTURE_CHANGE_TIME_FACTOR)
		{
			this->animateActiveTexture();
			this->activeTexture = BasicMonsterActiveTexture::RECEIVING_DAMAGE;
			this->activeTextureDirection = this->directedMovement.direction;
			this->lastTimeTextureWasRefreshed = now;
		}
	}
	else if (this->isMoving())
	{
		bool textureChanged = this->activeTexture != BasicMonsterActiveTexture::MOVING || this->activeTextureDirection != this->directedMovement.direction;
		if (textureChanged || (now - this->lastTimeTextureWasRefreshed) > TEXTURE_CHANGE_TIME_FACTOR)
		{
			this->animateActiveTexture();
			this->activeTexture = BasicMonsterActiveTexture::MOVING;
			this->activeTextureDirection = this->directedMovement.direction;
			this->lastTimeTextureWasRefreshed = now;
		}
	}
	else
	{
		bool textureChanged = this->activeTexture != BasicMonsterActiveTexture::STANDING || this->activeTextureDirection != this->directedMovement.direction;
		if (textureChanged || (now - this->lastTimeTextureWasRefreshed) > TEXTURE_CHANGE_TIME_FACTOR)
		{
			this->animateActiveTexture();
			this->activeTexture = BasicMonsterActiveTexture::STANDING;
			this->activeTextureDirection = this->directedMovement.direction;
			this->lastTimeTextureWasRefreshed = now;
		}
	}
}

void BasicMonster::animateActiveTexture()
{
	int newTextureIndex = this->getCurrentTextureIndex() + 1;
	int textureQuantity = this->getTextureQuantity();
	int begin = 0, end = 0;

	switch (this->activeTexture)
	{
		case BasicMonsterActiveTexture::DEAD:
			this->changeTextureIndex(0);
			break;

		case BasicMonsterActiveTexture::ATTACKING:
			switch (this->directedMovement.direction)
			{
				case BOTTOM_LEFT: begin = 1; end = textureQuantity; break;
				case BOTTOM_RIGHT: begin = 1 + textureQuantity; end = 2 * textureQuantity; break;
				case TOP_RIGHT: begin = 1 + 2 * textureQuantity; end = 3 * textureQuantity; break;
				case TOP_LEFT: begin = 1 + 3 * textureQuantity; end = 4 * textureQuantity; break;
			}
			break;
		case BasicMonsterActiveTexture::RECEIVING_DAMAGE:
			switch (this->directedMovement.direction)
			{
				case BOTTOM_LEFT: begin = 1 + 4 * textureQuantity; end = 5 * textureQuantity; break;
				case BOTTOM_RIGHT: begin = 1 + 5 * textureQuantity; end = 6 * textureQuantity; break;
				case TOP_RIGHT: begin = 1 + 6 * textureQuantity; end = 7 * textureQuantity; break;
				case TOP_LEFT: begin = 1 + 7 * textureQuantity; end = 8 * textureQuantity; break;
			}
			break;
		case BasicMonsterActiveTexture::MOVING:
			switch (this->directedMovement.direction)
			{
				case BOTTOM_LEFT: begin = 1 + 8 * textureQuantity; end = 9 * textureQuantity; break;
				case BOTTOM_RIGHT: begin = 1 + 9 * textureQuantity; end = 10 * textureQuantity; break;
				case TOP_RIGHT: begin = 1 + 10 * textureQuantity; end = 11 * textureQuantity; break;
				case TOP_LEFT: begin = 1 + 11 * textureQuantity; end = 12 * textureQuantity; break;
			}
			break;
		case BasicMonsterActiveTexture::STANDING:
			switch (this->directedMovement.direction)
			{
				case BOTTOM_LEFT: begin = 1 + 12 * textureQuantity; end = 13 * textureQuantity; break;
				case BOTTOM_RIGHT: begin = 1 + 13 * textureQuantity; end = 14 * textureQuantity; break;
				case TOP_RIGHT: begin = 1 + 14 * textureQuantity; end = 15 * textureQuantity; break;
				case TOP_LEFT: begin = 1 + 15 * textureQuantity; end = 16 * textureQuantity; break;
			}
			break;
	}

	if (this->isIntegerOnRange(newTextureIndex, begin, end))
		this->changeTextureIndex(newTextureIndex);
	else
		this->changeTextureIndex(begin);
}

bool BasicMonster::isIntegerOnRange(int integer, int begin, int end)
{
	return ((unsigned)(integer - begin) <= (end - begin)) ? true : false;
}

void BasicMonster::forceMonsterToLookAtPlayer(glm::vec3 playerPosition)
{
	glm::vec3 monsterPosition = this->getTransform()->getPosition();
	glm::vec3 differenceVector = playerPosition - monsterPosition;
	this->directedMovement.direction = this->ai->getDiagonalDirection(differenceVector);
}