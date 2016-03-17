#include "Projectile.h"
#include "LinkedTime.h"
#include "Map.h"
#include "Monster.h"
#include "Game.h"
#include "PacketController.h"
#include "Player.h"

Projectile::Projectile(Transform* transform, Mesh* mesh, Texture* texture, glm::vec3 direction, ProjectileType type, long ownerId) : Entity(transform, mesh, texture)
{
	this->originalPosition = transform->getPosition();
	this->direction = glm::normalize(direction);
	this->id = 0;
	this->type = type;
	this->dead = false;
	this->distance = 1;
	this->speed = 1;
	this->power = 10;
	this->ownerId = ownerId;
}

Projectile::~Projectile()
{

}

void Projectile::setSpeed(float speed)
{
	this->speed = speed;
}

bool Projectile::shouldBeDeleted()
{
	return this->dead;
}

ProjectileType Projectile::getType()
{
	return this->type;
}

void Projectile::setDistance(float distance)
{
	this->distance = distance;
}

int Projectile::getId()
{
	return this->id;
}

void Projectile::setId(int id)
{
	this->id = id;
}

void Projectile::update(Map* map, std::vector<Monster*>* monsters, Player* localPlayer, bool checkCollision)
{
	double now = LinkedTime::getTime();
	glm::vec3 newPos = this->transform->getPosition() + (this->direction * glm::vec3(this->speed, this->speed, this->speed));
	int hitMonsterIndex;

	// if projectile hit wall it must be deleted
	if (map->coordinateHasCollision(newPos))
	{
		this->dead = true;
	}
	// if projectile reached its limit distance it must be deleted.
	else if (glm::length(newPos - originalPosition) >= this->distance)
	{
		this->dead = true;
	}
	// projetil collision should be tested only if the game is single player OR is multiplayer but the player is the local player
	else if (checkCollision && this->type == ProjectileType::PLAYER_ATTACK)
	{
		if (monsterCollision(monsters, &hitMonsterIndex))
		{
			if (hitMonsterIndex >= 0)
			{
				Monster *hitMonster = (*monsters)[hitMonsterIndex];
				int damageOnMonster = (unsigned int)ceil(this->power / (hitMonster->getTotalDefense() / 10.0f));

				if (Game::multiplayer)
					PacketController::sendAttackCollisionToServer(hitMonster->getId(), this->id, damageOnMonster);

				this->dead = true;
				hitMonster->doDamage(damageOnMonster);
			}
		}
	}
	else if (checkCollision && this->type == ProjectileType::MONSTER_ATTACK)
	{
		glm::vec3 playerPos = localPlayer->getTransform()->getPosition();
		float playerSize = PLAYER_COLLISION_RANGE;

		// TODO : projetil na parede
		if (localPlayer->isAlive() && this->doesProjectileCollidedWithEntity(playerPos, playerSize))
		{
			int damageOnPlayer = (unsigned int)ceil(this->power / (localPlayer->getTotalDefense() / 10.0f));

			if (Game::multiplayer)
				PacketController::sendMonsterAttackCollisionToServer(ownerId, this->id, damageOnPlayer);

			this->dead = true;
			localPlayer->doDamage(damageOnPlayer);
		}
	}

	this->transform->translate(newPos.x, newPos.y, newPos.z);
}

bool Projectile::monsterCollision(std::vector<Monster*>* monsters, int* hitMonsterIndex)
{
	*hitMonsterIndex = -1;

	for (unsigned int i = 0; i < monsters->size(); i++)
	{
		if ((*monsters)[i]->isAlive())
		{
			glm::vec3 monsterPos = (*monsters)[i]->getTransform()->getPosition();
			float monsterSize = (*monsters)[i]->getTotalCollisionRange() / 10.0f;

			if (this->doesProjectileCollidedWithEntity(monsterPos, monsterSize))
			{
				*hitMonsterIndex = i;
				return true;
			}
		}
	}
	return false;
}

bool Projectile::doesProjectileCollidedWithEntity(glm::vec3 entityPosition, float entitySize)
{
	float difference = glm::length(glm::vec2(entityPosition) - glm::vec2(this->getTransform()->getPosition()));

	return difference < entitySize;
}

void Projectile::setPower(unsigned int power)
{
	this->power = power;
}