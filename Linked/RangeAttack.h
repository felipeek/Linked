#pragma once
#include <glm\glm.hpp>
#include <vector>

class Projectile;
class Entity;

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

	std::vector<Projectile*>* attacks;

	Entity* player;
};

