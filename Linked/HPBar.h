#pragma once
#include <glm\glm.hpp>
class Texture;
class Player;
class DynamicTexture;
class Entity;

#define NUMBARS 100

class HPBar
{
public:
	const glm::vec3 hpColor = glm::vec3(255, 0, 0);
	HPBar(Player* player);
	~HPBar();
	void update();
	void input();
	Entity* quad;
private:
	Player* player;
	DynamicTexture* hpBar;
	
	unsigned char* hpBarImage;
	unsigned int maxHP;
	unsigned int maxBars;
	unsigned int numBars;
	unsigned int prevBars;

	void setHP(unsigned int bars);
};

