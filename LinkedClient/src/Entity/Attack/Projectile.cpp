#include "Projectile.h"
#include "LinkedTime.h"

Projectile::Projectile(Transform* transform, Mesh* mesh, Texture* texture, float speed, glm::vec3 direction) : Entity(transform, mesh, texture)
{
	this->speed = (speed / 1000.0f);
	this->direction = glm::normalize(direction) * glm::vec3(speed, speed, speed);
	this->spawnTime = LinkedTime::getTime();
	this->id = 0;
}

Projectile::~Projectile()
{

}

double Projectile::getSpawnTime()
{
	return this->spawnTime;
}

int Projectile::getId()
{
	return this->id;
}

void Projectile::setId(int id)
{
	this->id = id;
}

void Projectile::update()
{
	glm::vec3 newPos = this->transform->getPosition() + this->direction;

	this->transform->translate(newPos.x, newPos.y, newPos.z);
}
