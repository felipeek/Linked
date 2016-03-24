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
	DistanceMonster(Transform* transform, Mesh* mesh, Texture* texture);
	~DistanceMonster();

	/* UPDATE & RENDER */
	virtual void update(Map* map, Player* player);
	virtual void render(Shader* shader);

	/* MOVEMENT */
	virtual void startOnlineMovement(glm::vec3 position);

	virtual void action(int actionId, int xid, glm::vec3 vector);

	virtual void attackCreature(Creature* creature);

	/* COPY */
	// if the "copy" parameter is NULL, it will allocate the monster
	// if not, it will just copy the attributes to the existing monster.
	virtual Monster* getCopy(Monster* copy);
};

