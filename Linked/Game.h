#pragma once
#include <vector>
#include "EntityMap.h"
#include "Camera.h"
#include "PrimitiveShader.h"
#include "MapShader.h"
#include "Time.h"

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
	double lastTime;

	void input();
	void printCoordinate(int x, int y);
};

