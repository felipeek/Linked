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
	Camera* camera;
	PrimitiveShader* shader;
	Light* light;
	EntityMap* entityMap;
	MapShader* mapShader;
	Map* map;
	Map* monsterMap;
	PlayerMovement* playerMovement;
	MonsterFactory* monsterFactory;
	std::vector<Monster*> monsters;
	double lastTime;

	void input();
	void printCoordinate(int x, int y);
};

