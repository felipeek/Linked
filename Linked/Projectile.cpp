#include "Projectile.h"
#include "Time.h"
#include "Display.h"

Projectile::Projectile(Transform* transform, Mesh* mesh, Texture* texture, float speed, glm::vec3 direction) : Entity(transform, mesh, texture)
{
	this->speed = (speed / 1000.0f);
	this->direction = glm::normalize(direction) * glm::vec3(speed, speed, speed);
	spawnTime = Time::getTime();
}


Projectile::~Projectile()
{

}

void Projectile::update()
{
	glm::vec3 newPos = transform->getPosition() + direction;

	transform->translate(newPos.x, newPos.y, newPos.z);
}
