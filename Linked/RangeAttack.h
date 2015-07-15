#pragma once
#include <glm\glm.hpp>
#include <vector>

#define ASPD 0.1

class Projectile;
class Entity;
class Texture;

class RangeAttack
{
public:
	RangeAttack(Entity* player, std::vector<Projectile*>* attacks);
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

	Entity* player;
	Texture* texture;
};

