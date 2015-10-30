#pragma once
#include <glm\glm.hpp>
class Player;
class DynamicTexture;
class Entity;
class Mesh;

#define NUMBARS 100
#define BACKGROUND 255		// 255(black) 0(transparent)
#define MIPMAPBIAS -10

class HPBar
{
public:
	glm::vec3 hpColor;
	const glm::vec3 highHP = glm::vec3(0,255,0);
	const glm::vec3 mediumHP = glm::vec3(180,255,0);
	const glm::vec3 lowHP = glm::vec3(255,0,0);
	const glm::vec3 backgroundHP = glm::vec3(30, 30, 30);	// background color

	HPBar(Player* player);
	~HPBar();
	void update();
	void input();
	Entity* quad;
private:
	Player* player;
	DynamicTexture* hpBar;
	Mesh* mesh;
	
	unsigned char* hpBarImage;
	unsigned int maxHP;
	unsigned int maxBars;
	unsigned int numBars;
	unsigned int prevBars;

	void setHP(unsigned int bars);
};

