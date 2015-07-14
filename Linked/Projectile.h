#pragma once
#include <glm\glm.hpp>
#include "Entity.h"

class Projectile : public Entity
{
public:
	Projectile(Transform* transform, Mesh* mesh, Texture* texture, float speed, glm::vec3 direction);
	~Projectile();
	void update();
private:
	glm::vec3 direction;
	float speed;
};

