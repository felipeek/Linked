#include "BasicMonster.h"
#include "Player.h"
#include "MonsterAI.h"
#include "Map.h"
#include "Game.h"
#include "PacketController.h"
#include "LinkedTime.h"

BasicMonster::BasicMonster()
{
}

BasicMonster::~BasicMonster()
{
}

void BasicMonster::update(Map* map, std::vector<Player*>* players)
{
	glm::vec3 currentPosition = this->getPosition();
	Monster::update(map, players);

	if (players->size() > 0)
	{
		Player* player = (*players)[0];
		double now = LinkedTime::getTime();

		if (this->isAlive())
		{
			if (this->ai->isOnRangeToAttack(player->getPosition()) && player->isAlive())
			{
				if (!this->isAttacking()) this->attackCreature(player);
				this->stop(); // For Texture Management
			}
			else if (this->isAttacking() || this->isReceivingDamage())
			{
				this->stop();
			}
			else if (this->ai->isOnRangeToChaseTarget(player->getPosition()) && this->ai->isPathFreeOfCollisions(map, player->getPosition()) && player->isAlive())
			{
				this->moveToAttackPlayer(map, player);
				this->movingRandomly = false;
				this->move(this->directedMovement.direction); // For Texture Management
			}
			else if (!this->ai->shouldStandStill())
			{
				this->moveRandomly(map, player);
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

Monster* BasicMonster::getCopy(Monster* copy)
{
	// Create new monster if necessary.
	if (copy == nullptr) copy = new BasicMonster();

	return Monster::getCopy(copy);
}

void BasicMonster::moveToAttackPlayer(Map* map, Player* player)
{
	glm::vec3 newMonsterPosition = this->ai->getNextStep(this->directedMovement.movement);

	// If the monster is not moving towards the player, starts a new movement towards the player.
	if (!this->movingToAttackPlayer)
	{
		this->directedMovement = this->ai->generateMovementTowardsCoordinate(player->getPosition());
		this->movingToAttackPlayer = true;
		if (!map->coordinateHasCollision(newMonsterPosition))
			this->setPosition(glm::vec3(newMonsterPosition.x, newMonsterPosition.y, this->getPosition().z));
		else
			this->movingToAttackPlayer = false;
	}
	// If the monster was already moving towards the player and reached the destination, it stops moving towards the player.
	if (this->ai->reachDestination(newMonsterPosition, this->directedMovement.movement) && this->movingToAttackPlayer)
	{
		this->directedMovement = this->ai->generateMovementTowardsCoordinate(player->getPosition());
		this->movingToAttackPlayer = true;
		if (!map->coordinateHasCollision(newMonsterPosition))
			this->setPosition(glm::vec3(newMonsterPosition.x, newMonsterPosition.y, this->getPosition().z));
		else
			this->movingToAttackPlayer = false;
	}
	// If the monster was already moving towards the player and shouldn't stop, it continues moving towards the player.
	else
	{
		if (!map->coordinateHasCollision(newMonsterPosition))
			this->setPosition(glm::vec3(newMonsterPosition.x, newMonsterPosition.y, this->getPosition().z));
		else
			this->movingToAttackPlayer = false;
	}
}

void BasicMonster::moveRandomly(Map* map, Player* player)
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
			this->setPosition(glm::vec3(newMonsterPosition.x, newMonsterPosition.y, this->getPosition().z));
		else
		{
			this->ai->resetStandStill();
			this->movingRandomly = false;
		}
	}
}

/* NETWORK MOVEMENT */

bool BasicMonster::mustUpdateDestinationToClients()
{
	glm::vec3 oldMov = this->oldDirectedMovement.movement;
	glm::vec3 curMov = this->directedMovement.movement;

	if (oldMov.x != curMov.x || oldMov.y != curMov.y)
		return true;
	else
		return false;
}

glm::vec3 BasicMonster::getDestination()
{
	this->oldDirectedMovement = this->directedMovement;
	return this->directedMovement.movement;
}