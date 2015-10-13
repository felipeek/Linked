#pragma once
#include <glm\glm.hpp>
#include <vector>


#define ASPD 0.3f
#define ATTACK 20
#define LIFE 1.0f
#define SPEED 0.4f

class Projectile;
class Player;
class Texture;
class Monster;
class Map;
class Mesh;

class RangeAttack
{
public:
	RangeAttack(Player* player, std::vector<Projectile*>* attacks, std::vector<Monster*>* monsters, Map* map);
	~RangeAttack();

	void createProjectile(glm::vec3 direction);
	void setSpeed(float value);
	void setLife(float value);

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
	bool createProjectileDirectedToMouse();
	void sendAttackToServer();

	Player* player;
	Texture* texture;
	Mesh* mesh;
};

