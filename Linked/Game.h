#pragma once

class Game
{
public:
	Game();
	~Game();
	void render();
	void update();
private:
	void input();
	void test(int x, int y);
};
