#pragma once
#include <iostream>
#include <vector>
#include "glm/glm.hpp"

#define MAP_SIZE 1024
#define GAMESPEED 60.0

class MonsterFactory;
class GameEntityFactory;
class Map;
class Player;
class Monster;
class Projectile;

class Game
{
public:
	Game();
	~Game();
	void update();
	Player* createNewPlayer(unsigned short clientId);
	Projectile* createNewProjectile(glm::vec3 position, glm::vec3 direction, int playerId);

	/* AUXILIAR METHODS */
	Monster* getMonsterOfId(int id);
	void destroyProjectileOfId(int id);
private:
	MonsterFactory* monsterFactory;
	GameEntityFactory* gameEntityFactory;
	Map* map;
	std::vector<Player*> players;
	std::vector<Monster*> monsters;
	std::vector<Projectile*> projectiles;

	void createMap();
	void createPlayers();
	int createMonsters();
};