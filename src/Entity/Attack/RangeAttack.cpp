#include "RangeAttack.h"
#include "Projectile.h"
#include "Input.h"
#include "Texture.h"
#include "LinkedTime.h"
#include "Primitive.h"
#include "Monster.h"
#include "Map.h"
#include "Mesh.h"
#include "Player.h"
#include "PacketController.h"
#include "Game.h"

#include <iostream>

RangeAttack::RangeAttack(Player* player, std::vector<Projectile*>* attacks, std::vector<Monster*>* monsters, Map* map)
{
	this->mesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 0.3f, 0.3f));
	this->player = player;
	this->attacks = attacks;
	this->monsters = monsters;
	this->map = map;
	texture = new Texture("./res/Textures/energyBall.png");
	lastTimeUpdate = 0;
	lastTimeCreate = 0;
	setLife(LIFE);
	setSpeed(SPEED);
}

RangeAttack::~RangeAttack()
{
}

std::vector<Projectile*>* RangeAttack::getAttacks()
{
	return this->attacks;
}

void RangeAttack::createProjectile(glm::vec3 direction, int projId)
{
	glm::vec3 playerPos = player->getTransform()->getPosition();
	playerPos.z = 0;
	direction.z = 1.0f;

	Transform* projectileTransform = new Transform(playerPos + glm::vec3(0, 0, playerPos.z), 35, glm::vec3(1, 0, 0), glm::vec3(3, 3, 3));
	Projectile* entityD = new Projectile(projectileTransform, mesh, texture, speed, direction);
	entityD->setId(projId);
	(*attacks).push_back(entityD);

	//std::cout << "created projectile " << projId << "." << std::endl;
}

void RangeAttack::setSpeed(float value)
{
	speed = value;
}

void RangeAttack::setLife(float value)
{
	life = value;
}

bool RangeAttack::monsterCollision(Projectile* projectile, int* hitMonsterIndex)
{
	glm::vec3 projPosition = projectile->getTransform()->getPosition();

	*hitMonsterIndex = -1;

	for (unsigned int i = 0; i < monsters->size(); i++)
	{
		if ((*monsters)[i]->isAlive())
		{
			glm::vec3 monsterPos = (*monsters)[i]->getTransform()->getPosition();
			float monsterSize = (*monsters)[i]->getTotalCollisionRange() / 10.0f;

			float difference = glm::length(glm::vec2(monsterPos) - glm::vec2(projPosition));

			// TODO : projetil na parede
			if (difference < monsterSize)
			{
				*hitMonsterIndex = i;
				return true;
			}
		}
	}
	return false;
}

bool RangeAttack::createProjectileDirectedToMouse()
{
	double now = Time::getTime();

	if (now - lastTimeCreate >= ASPD)
	{
		glm::vec3 playerPos = player->getTransform()->getPosition();
		glm::vec3 direction = Input::mouseAttack.getMouseIntersection() - playerPos;

		this->createProjectile(direction, 0);

		lastTimeCreate = now;

		return true;
	}

	return false;
}

void RangeAttack::sendAttackToServer()
{
	double now = Time::getTime();

	if (now - lastTimeCreate >= ASPD)
	{
		glm::vec3 playerPos = player->getTransform()->getPosition();
		glm::vec3 direction = Input::mouseAttack.getMouseIntersection() - playerPos;

		PacketController::sendAttackToServer(direction);

		lastTimeCreate = now;
	}
}

void RangeAttack::update()
{
	double now = Time::getTime();
	int hitMonsterIndex;

	for (unsigned int i = 0; i < attacks->size(); i++)
	{
		(*attacks)[i]->update();

		// if projectile hit wall it must be deleted
		if (map->coordinateHasCollision((*attacks)[i]->getTransform()->getPosition()))
		{
			delete (*attacks)[i];
			attacks->erase((*attacks).begin() + i);
		}
		// if projectile lived long enough it must be deleted
		else if (now - (*attacks)[i]->getSpawnTime() >= life)
		{
			delete (*attacks)[i];
			attacks->erase((*attacks).begin() + i);
		}
		// projetil collision should be tested only if the game is single player OR is multiplayer but the player is the local player
		else if ((Game::multiplayer && player->getType() == LOCAL) || !Game::multiplayer)
		{
			if (monsterCollision((*attacks)[i], &hitMonsterIndex))
			{
				if (hitMonsterIndex >= 0)
				{
					Monster *hitMonster = (*monsters)[hitMonsterIndex];

					if (Game::multiplayer)
						PacketController::sendAttackCollisionToServer(hitMonster->getId(), (*attacks)[i]->getId());

					delete (*attacks)[i];
					attacks->erase((*attacks).begin() + i);
					hitMonster->doDamage(ATTACK);
				}
			}
		}
	}
}

void RangeAttack::input()
{
	if (Input::attack && player->isAlive())
	{
		if (Game::multiplayer)
		{
			this->sendAttackToServer();
		}
		else
		{
			createProjectileDirectedToMouse();
			player->doAttack();
		}
	}
}
