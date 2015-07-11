#pragma once

class Game
{
public:
	Game(int windowsWidth, int windowsHeight);
	~Game();
	void render();
	void update();
private:
	void input();
	void test(int x, int y);
};
