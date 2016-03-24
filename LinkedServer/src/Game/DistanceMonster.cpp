#include "DistanceMonster.h"
#include "Projectile.h"
#include "Player.h"
#include "LinkedTime.h"
#include "Game.h"
#include "MonsterAI.h"

DistanceMonster::DistanceMonster() : Monster(), BasicMonster(), TurretMonster()
{
}

DistanceMonster::~DistanceMonster()
{
}

void DistanceMonster::attackCreature(Creature* creature)
{
	TurretMonster::tryToCreateProjectile((Player*)creature);
	this->attack();
}

void DistanceMonster::update(Map* map, std::vector<Player*>* players)
{
	Monster::update(map, players);
	BasicMonster::updateMovement(map, players);
}

Monster* DistanceMonster::getCopy(Monster* copy)
{
	// Create new monster if necessary.
	if (copy == nullptr) copy = new DistanceMonster();

	return Monster::getCopy(copy);
}

/* NETWORK MOVEMENT */

bool DistanceMonster::mustUpdateDestinationToClients()
{
	return BasicMonster::mustUpdateDestinationToClients();
}

glm::vec3 DistanceMonster::getDestination()
{
	return BasicMonster::getDestination();
}