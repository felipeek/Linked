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
	TurretMonster(Transform* transform, Mesh* mesh, Texture* texture);
	~TurretMonster();

	/* UPDATE & RENDER */
	virtual void update(Map* map, Player* player);
	virtual void render(Shader* shader);

	/* MOVEMENT */
	virtual void startOnlineMovement(glm::vec3 position);

	virtual void action(glm::vec3 vector);

	/* COPY */
	// if the "copy" parameter is NULL, it will allocate the monster
	// if not, it will just copy the attributes to the existing monster.
	virtual Monster* getCopy(Monster* copy);

private:
	void tryToCreateProjectile(Player* player);
	std::vector<Projectile*> projectiles;
	void createProjectile(glm::vec3 direction, int projId);
	Mesh* projectileMesh;
	Texture* projectileTexture;
	double lastProjectileTime = 0;
};

