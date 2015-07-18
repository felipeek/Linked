#pragma once
#include <vector>
#define MAP_SIZE 1024

class EntityMap;
class Entity;
class Camera;
class PrimitiveShader;
class MapShader;
class PlayerMovement;
class Map;
class Light;
class MonsterFactory;
class Monster;
class RangeAttack;
class Projectile;
class GameEntityFactory;
class GameEntity;
class Player;

class Game
{
public:
	Game();
	Game(int windowsWidth, int windowsHeight);
	~Game();
	void render();
	void update();
private:
	std::vector<Entity*> entities;
	std::vector<Projectile*> attacks;
	std::vector<GameEntity*> gameEntities;
	Camera* camera;
	PrimitiveShader* shader;
	Light* light;
	EntityMap* entityMap;
	MapShader* mapShader;
	Map* map;
	PlayerMovement* playerMovement;
	MonsterFactory* monsterFactory;
	GameEntityFactory* gameEntityFactory;
	std::vector<Monster*> monsters;
	Player* player;
	RangeAttack* rangeAttack;
	double lastTime;

	void input();
	void printCoordinate(int x, int y);
};

