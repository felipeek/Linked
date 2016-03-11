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
	Projectile(Transform* transform, Mesh* mesh, Texture* texture, glm::vec3 direction, ProjectileType type);
	~Projectile();

	int getId();
	void setId(int id);
	ProjectileType getType();

	void update(Map* map, std::vector<Monster*>* monsters, Player* localPlayer, bool checkCollision);
	bool shouldBeDeleted();

	void setDistance(float distance);
	void setSpeed(float speed);
	void setPower(unsigned int power);
private:
	glm::vec3 direction;
	glm::vec3 originalPosition;
	float speed;
	int id;
	ProjectileType type;
	bool dead;
	float distance;
	unsigned int power;

	bool monsterCollision(std::vector<Monster*>* monsters, int* hitMonsterIndex);
	bool doesProjectileCollidedWithEntity(glm::vec3 entityPosition, float entitySize);
};

