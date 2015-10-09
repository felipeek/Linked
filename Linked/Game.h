#pragma once
#include "Configuration.h"
#include <vector>
#include "network\UDPClient.h"
#include <glm\glm.hpp>

#define MAP_SIZE 1024

class EntityMap;
class Entity;
class Camera;
class PrimitiveShader;
class CommonShader;
class GUIShader;
class MapShader;
class Map;
class Light;
class MonsterFactory;
class Monster;
class RangeAttack;
class Projectile;
class GameEntityFactory;
class GameEntity;
class Player;
class GUI;

class Game
{
public:
	Game();
	Game(int windowsWidth, int windowsHeight);
	~Game();
	void render();
	void update();

	static glm::vec3 pos;
private:
	std::vector<Entity*> entities;
	std::vector<Projectile*> attacks;
	std::vector<GameEntity*> gameEntities;

	Camera* camera;
	Light* light;
	CommonShader* commonShader;
	PrimitiveShader* primitiveShader;
	GUIShader* fontShader;
	MapShader* mapShader;

	EntityMap* entityMap;
	Map* map;

	Player* player;

#ifdef MULTIPLAYER
	Player* secondPlayer;
	std::vector<Projectile*> secondPlayerAttacks;
#endif

	MonsterFactory* monsterFactory;
	GameEntityFactory* gameEntityFactory;
	std::vector<Monster*> monsters;

	RangeAttack* rangeAttack;

	GUI* gui;

	UDPClient* udpClient;

	double lastTime;

	void input();
	void printCoordinate(int x, int y);
};

