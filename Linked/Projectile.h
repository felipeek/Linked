#pragma once
#include <glm\glm.hpp>
#include "Entity.h"

class Projectile : public Entity
{
public:
	Projectile(Transform* transform, Mesh* mesh, Texture* texture, float speed, glm::vec3 direction);
	~Projectile();

	int getId();
	void setId(int id);
	double getSpawnTime();
	
	void update();
private:
	glm::vec3 direction;
	float speed;
	int id;
	double spawnTime;
};

