#include "Projectile.h"
#include "LinkedTime.h"
#include "Map.h"
#include "Monster.h"
#include "Game.h"
#include "PacketController.h"
#include "Player.h"

Projectile::Projectile(Transform* transform, Mesh* mesh, Texture* texture, float speed, glm::vec3 direction, ProjectileType type) : Entity(transform, mesh, texture)
{
	this->speed = (speed / 1000.0f);
	this->direction = glm::normalize(direction) * glm::vec3(speed, speed, speed);
	this->spawnTime = LinkedTime::getTime();
	this->id = 0;
	this->type = type;
	this->dead = false;
	this->life = 1;
}

Projectile::~Projectile()
{

}

bool Projectile::shouldBeDeleted()
{
	return this->dead;
}

ProjectileType Projectile::getType()
{
	return this->type;
}

double Projectile::getSpawnTime()
{
	return this->spawnTime;
}

void Projectile::setLife(float life)
{
	this->life = life;
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
	glm::vec3 newPos = this->transform->getPosition() + this->direction;
	int hitMonsterIndex;

	// if projectile hit wall it must be deleted
	if (map->coordinateHasCollision(newPos))
	{
		this->dead = true;
	}
	// if projectile lived long enough it must be deleted
	else if ((now - this->getSpawnTime()) >= this->life)
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

				if (Game::multiplayer)
					PacketController::sendAttackCollisionToServer(hitMonster->getId(), this->id);

				this->dead = true;
				hitMonster->doDamage(20);
			}
		}
	}
	else if (checkCollision && this->type == ProjectileType::MONSTER_ATTACK)
	{
		glm::vec3 playerPos = localPlayer->getTransform()->getPosition();
		float playerSize = PLAYER_COLLISION_RANGE;

		// TODO : projetil na parede
		if (this->doesProjectileCollidedWithEntity(playerPos, playerSize))
		{
			localPlayer->doDamage(10);
			this->dead = true;
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