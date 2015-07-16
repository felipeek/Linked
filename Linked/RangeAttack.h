#pragma once
#include <glm\glm.hpp>
#include <vector>


#define ASPD 0.1
#define ATTACK 10

class Projectile;
class Entity;
class Texture;
class Monster;
class Map;
class Mesh;

class RangeAttack
{
public:
	RangeAttack(Entity* player, std::vector<Projectile*>* attacks, std::vector<Monster*>* monsters, Map* map);
	~RangeAttack();

	void setSpeed(float value);
	void setLife(float value);
	void attack();
	void update();
private:
	float speed;
	float life;
	double lastTimeUpdate;
	double lastTimeCreate;
	std::vector<Projectile*>* attacks;
	std::vector<Monster*>* monsters;
	Map* map;

	bool monsterCollision(Projectile* projectile, int* hitMonsterIndex);

	Entity* player;
	Texture* texture;
	Mesh* mesh;
};

