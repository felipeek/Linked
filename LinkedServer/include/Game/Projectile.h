#pragma once
#include "glm/glm.hpp"
#include "WorldObject.h"

class Projectile : public WorldObject
{
public:
	Projectile(glm::vec3 position, glm::vec3 direction, int playerId);
	~Projectile();

	static int G_ID;

	int getId();
	void setId(int id);
	int getPlayerId();
	glm::vec3 getDirection();
private:
	glm::vec3 direction;
	int id;
	int playerId;
};

