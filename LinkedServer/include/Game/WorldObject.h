#pragma once
#include <glm\glm.hpp>

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
	bool didPositionChanged();
	void resetPositionChanged();
private:
	glm::vec3 position;
	bool positionChanged;
	long id;
	static unsigned short NEXT_ID;
};

