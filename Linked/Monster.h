#pragma once
#include "Entity.h"

class Monster : public Entity
{
public:
	Monster(Transform* transform, Mesh* mesh);
	~Monster();
	std::string getName();
	void setName(std::string name);
	unsigned int getHp();
	void setHp(unsigned int hp);
	unsigned int getAttack();
	void setAttack(unsigned int attack);
	unsigned int getDefense();
	void setDefense(unsigned int defense);
	int getMapColorRed();
	void setMapColorRed(int red);
	int getMapColorGreen();
	void setMapColorGreen(int green);
	int getMapColorBlue();
	void setMapColorBlue(int blue);
	glm::vec3 getMapColor();
	void setMapColor(glm::vec3 mapColor);
private:
	std::string name;
	unsigned int hp;
	unsigned int attack;
	unsigned int defense;
	glm::vec3 mapColor;
};