#pragma once
#include <vector>
#include "Entity.h"
#include "Camera.h"
#include "PrimitiveShader.h"
#include "MapShader.h"

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

	void input();
	void printCoordinate(int x, int y);
};
