#pragma once
#include <glm\glm.hpp>
#include "Entity.h"

#define PLAYER_COLLISION_RANGE 1

class Map;
class Monster;
class Player;

enum class ProjectileType
{
	MONSTER_ATTACK,
	PLAYER_ATTACK
};

class Projectile : public Entity
{
public:
	Projectile(Transform* transform, Mesh* mesh, Texture* texture, float speed, glm::vec3 direction, ProjectileType type);
	~Projectile();

	int getId();
	void setId(int id);
	double getSpawnTime();
	ProjectileType getType();

	void update(Map* map, std::vector<Monster*>* monsters, Player* localPlayer, bool checkCollision);
	bool shouldBeDeleted();
	void setLife(float life);
private:
	glm::vec3 direction;
	float speed;
	int id;
	double spawnTime;
	ProjectileType type;
	bool dead;
	float life;

	bool monsterCollision(std::vector<Monster*>* monsters, int* hitMonsterIndex);
	bool doesProjectileCollidedWithEntity(glm::vec3 entityPosition, float entitySize);
};

