#pragma once
#include "BasicMonster.h"
#include "TurretMonster.h"

//#define PROJECTILE_CHANCE 0.2f // in % (from 0.01f to 99.99f)
#define DISTANCE_MONSTER_PROJECTILE_TIME 0.5f
#define DISTANCE_MONSTER_PROJECTILE_RANGE 30.0f
#define DISTANCE_MONSTER_PROJECTILE_SPEED 0.4f

class Projectile;
class Player;
class Audio;

class DistanceMonster : public BasicMonster, public TurretMonster
{
public:
	DistanceMonster();
	~DistanceMonster();

	/* UPDATE & RENDER */
	virtual void update(Map* map, std::vector<Player*>* players);

	/* NETWORK MOVEMENT */
	virtual bool mustUpdateDestinationToClients();
	virtual glm::vec3 getDestination();

	virtual void attackCreature(Creature* creature);

	/* COPY */
	// if the "copy" parameter is NULL, it will allocate the monster
	// if not, it will just copy the attributes to the existing monster.
	virtual Monster* getCopy(Monster* copy);
};

