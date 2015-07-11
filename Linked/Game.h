#pragma once

class Game
{
public:
	Game();
	Game(int windowsWidth, int windowsHeight);
	~Game();
	void render();
	void update();
private:
	void input();
	void printCoordinate(int x, int y);
};
