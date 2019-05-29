#pragma once
#include "glm/glm.hpp"

#define FIRST_ID 1

class WorldObject
{
public:
	WorldObject();
	~WorldObject();
	glm::vec3 getPosition();
	void setPosition(glm::vec3 position);
	unsigned short getId();
	void setId(unsigned short id);
private:
	glm::vec3 position;
	long id;
	static unsigned short NEXT_ID;
};

