#pragma once

class Player;

class GUI
{
public:
	GUI(Player* player);
	~GUI();
private:
	Player* player;
};

