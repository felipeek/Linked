#pragma once
#include "Entity.h"
class MonsterAI;
class Map;

class Monster : public Entity
{
public:
	Monster(Transform* transform, Mesh* mesh, Texture* texture);
	~Monster();
	std::string getName();
	void setName(std::string name);
	unsigned int getHp();
	void setHp(unsigned int hp);
	unsigned int getAttack();
	void setAttack(unsigned int attack);
	unsigned int getDefense();
	void setDefense(unsigned int defense);
	unsigned int getSpeed();
	void setSpeed(unsigned int speed);
	int getMapColorRed();
	void setMapColorRed(int red);
	int getMapColorGreen();
	void setMapColorGreen(int green);
	int getMapColorBlue();
	void setMapColorBlue(int blue);
	glm::vec3 getMapColor();
	void setMapColor(glm::vec3 mapColor);
	void moveTo(Entity* entity, Map* map);
private:
	std::string name;
	unsigned int hp;
	unsigned int attack;
	unsigned int defense;
	unsigned int speed;
	glm::vec3 mapColor;
	MonsterAI* ai;
};