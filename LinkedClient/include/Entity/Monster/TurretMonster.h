#pragma once
#include "Monster.h"

//#define PROJECTILE_CHANCE 0.2f // in % (from 0.01f to 99.99f)
#define TURRET_MONSTER_PROJECTILE_TIME 0.5f
#define TURRET_MONSTER_PROJECTILE_RANGE 30.0f
#define TURRET_MONSTER_PROJECTILE_SPEED 0.6f

class Projectile;
class Player;
class Audio;

class TurretMonster : virtual public Monster
{
public:
	TurretMonster(Transform* transform, Mesh* mesh, Texture* texture);
	~TurretMonster();

	/* UPDATE & RENDER */
	virtual void update(Map* map, Player* player, std::vector<Monster*>* monsters);
	virtual void render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer);

	/* MOVEMENT */
	virtual void startOnlineMovement(glm::vec3 position);

	virtual void action(int actionId, int xid, glm::vec3 vector);

	/* COPY */
	// if the "copy" parameter is NULL, it will allocate the monster
	// if not, it will just copy the attributes to the existing monster.
	virtual Monster* getCopy(Monster* copy);

protected:
	void updateProjectiles(Map* map, Player* player);
	void tryToCreateProjectile(Player* player);

private:
	std::vector<Projectile*> projectiles;
	void createProjectile(glm::vec3 direction, int projId);
	void destroyProjectile(int projId);
	Mesh* projectileMesh;
	Texture* projectileTexture;
	double lastProjectileTime = 0;
	Audio* attackSound;
};

