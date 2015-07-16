#pragma once
#include <glm\glm.hpp>
#include <vector>


#define ASPD 0.1f
#define ATTACK 20
#define LIFE 1.0f
#define SPEED 0.2f

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
	void input();
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

