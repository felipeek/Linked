#pragma once
#include "Entity.h"
#include <string>

class GameEntity : public Entity
{
public:
	GameEntity(Transform* transform, Mesh* mesh, Texture* texture);
	~GameEntity();
	std::string getName();
	void setName(std::string name);
	unsigned int getSize();
	void setSize(unsigned int size);
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
	glm::vec3 mapColor;
	unsigned int size;
};

