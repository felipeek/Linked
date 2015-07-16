#include "RangeAttack.h"
#include "Projectile.h"
#include "Input.h"
#include "Texture.h"
#include "Time.h"
#include "Primitive.h"
#include "Monster.h"
#include "Map.h"
#include "Mesh.h"

#include <iostream>

RangeAttack::RangeAttack(Entity* player, std::vector<Projectile*>* attacks, std::vector<Monster*>* monsters, Map* map)
{
	this->mesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 0.3f, 0.3f));
	this->player = player;
	this->attacks = attacks;
	this->monsters = monsters;
	this->map = map;
	texture = new Texture("./res/Textures/clownAtlas.png", 2, 2);
	lastTimeUpdate = 0;
	lastTimeCreate = 0;
}

RangeAttack::~RangeAttack()
{
}

void RangeAttack::update()
{

	double now = Time::getTime();
	int hitMonsterIndex;
	
	bool piercingAttack = false;

	for (unsigned int i = 0; i < attacks->size(); i++)
	{
		(*attacks)[i]->update();
	
		if (monsterCollision((*attacks)[i], &hitMonsterIndex))
		{
			if (hitMonsterIndex >= 0)
			{
				if (ATTACK >= (*monsters)[hitMonsterIndex]->getHp())
				{
					delete (*monsters)[hitMonsterIndex];
					monsters->erase((*monsters).begin() + hitMonsterIndex);
				}
				else
				{
					(*monsters)[hitMonsterIndex]->setHp((*monsters)[hitMonsterIndex]->getHp() - ATTACK);
				}
			}
			if (!piercingAttack)
			{
				delete (*attacks)[i];
				attacks->erase((*attacks).begin() + i);
			}
		}
		else if (now - (*attacks)[i]->spawnTime >= life)
		{
			delete (*attacks)[i];
			attacks->erase((*attacks).begin() + i);
		}
	}
}

void RangeAttack::attack()
{
	glm::vec3 playerPos = player->getTransform()->getPosition();
	glm::vec3 direction = Input::mouseAttack.attack();
	direction.z = 0;
	direction = glm::normalize(direction);

	double now = Time::getTime();

	if (now - lastTimeCreate >= ASPD)
	{
		Projectile* entityD = new Projectile(new Transform(playerPos + glm::vec3(0,0,playerPos.z), 35, glm::vec3(1, 0, 0), glm::vec3(3, 3, 3)), mesh, texture, speed, direction);
		(*attacks).push_back(entityD);
		lastTimeCreate = now;
	}
	
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
		glm::vec3 monsterPos = (*monsters)[i]->getTransform()->getPosition();
		float monsterSize = (*monsters)[i]->getCollisionRange()/10.0f;
		
		float difference = glm::length(glm::vec2(monsterPos) - glm::vec2(projPosition));

		// TODO : projetil na parede
		if (difference < monsterSize)
		{
			*hitMonsterIndex = i;
			return true;
		}
		if (MapTerrainImageLoader::isOfCollisionType(map->getMapCoordinateForMapCreation(projPosition).terrain))
		{
			return true;
		}
	}
	return false;
}

void RangeAttack::input()
{
	if (Input::attack)
	{
		setLife(LIFE);
		setSpeed(SPEED);
		attack();
	}
}