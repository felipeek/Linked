#include "Projectile.h"


Projectile::Projectile(Transform* transform, Mesh* mesh, Texture* texture, float speed, glm::vec3 direction) : Entity(transform, mesh, texture)
{
	this->speed = speed;
	this->direction = direction;
}


Projectile::~Projectile()
{
}

void Projectile::update()
{
	glm::vec3 newPos = transform->getPosition() + direction;

	transform->translate(newPos.x, newPos.y, newPos.z);
}
