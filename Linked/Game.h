#pragma once
#include <vector>
class EntityMap;
class Entity;
class Camera;
class PrimitiveShader;
class MapShader;
class PlayerMovement;
class Map;
class Light;

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
	PlayerMovement* playerMovement;
	double lastTime;

	void input();
	void printCoordinate(int x, int y);
};

