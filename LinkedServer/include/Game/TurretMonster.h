#pragma once
#pragma once
#include "Monster.h"

//#define PROJECTILE_CHANCE 0.2f // in % (from 0.01f to 99.99f)
#define TURRET_MONSTER_PROJECTILE_TIME 0.5f
#define TURRET_MONSTER_PROJECTILE_RANGE 30.0f
#define TURRET_MONSTER_PROJECTILE_SPEED 0.6f

class Projectile;
class Player;

class TurretMonster : public Monster
{
public:
	TurretMonster();
	~TurretMonster();

	/* UPDATE & RENDER */
	virtual void update(Map* map, std::vector<Player*>* players);

	/* NETWORK MOVEMENT */
	virtual bool mustUpdateDestinationToClients();
	virtual glm::vec3 getDestination();

	/* COPY */
	// if the "copy" parameter is NULL, it will allocate the monster
	// if not, it will just copy the attributes to the existing monster.
	virtual Monster* getCopy(Monster* copy);

private:
	void tryToCreateProjectile(Player* player);
	void createProjectile(glm::vec3 direction, int projId, int playerId);
	double lastProjectileTime = 0;
};

