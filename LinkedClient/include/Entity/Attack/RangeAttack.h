#pragma once
#include <glm\glm.hpp>
#include <vector>

#define PLAYER_ASPD 0.3f
#define PLAYER_PROJECTILE_DISTANCE 20
#define PLAYER_PROJECTILE_SPEED 0.4f

class Projectile;
class Player;
class Texture;
class Monster;
class Map;
class Mesh;

class RangeAttack
{
public:
	RangeAttack(Player* player, std::vector<Projectile*>* attacks);
	~RangeAttack();

	std::vector<Projectile*>* getAttacks();
	void createProjectile(glm::vec3 direction, int projId);

	void update(Map* map, std::vector<Monster*>* monsters);
	void input();
private:
	double lastTimeUpdate;
	double lastTimeCreate;
	std::vector<Projectile*>* attacks;

	bool createProjectileDirectedToMouse();
	void sendAttackToServer();

	Player* player;
	Texture* texture;
	Mesh* mesh;
};

