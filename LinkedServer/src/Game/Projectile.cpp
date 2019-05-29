#include "Projectile.h"

int Projectile::G_ID = 0;

Projectile::Projectile(glm::vec3 position, glm::vec3 direction, int playerId)
{
	this->direction = direction;
	this->setPosition(position);
	this->setId(Projectile::G_ID);
	this->playerId = playerId;
	G_ID++;
}


Projectile::~Projectile()
{

}

int Projectile::getPlayerId()
{
	return this->playerId;
}

int Projectile::getId()
{
	return this->id;
}

void Projectile::setId(int id)
{
	this->id = id;
}

glm::vec3 Projectile::getDirection()
{
	return this->direction;
}