#include "DistanceMonster.h"
#include "Projectile.h"
#include "Player.h"
#include "LinkedTime.h"
#include "Audio.h"
#include "Game.h"
#include "MonsterAI.h"

DistanceMonster::DistanceMonster(Transform* transform, Mesh* mesh, Texture* texture) : Monster(transform, mesh, texture), BasicMonster(transform, mesh, texture), TurretMonster(transform, mesh, texture)
{
}

DistanceMonster::~DistanceMonster()
{
}

void DistanceMonster::attackCreature(Creature* creature)
{
	TurretMonster::tryToCreateProjectile((Player*)creature);
	this->attack();
}

void DistanceMonster::update(Map* map, Player* player)
{
	Monster::update(map, player);
	BasicMonster::updateMovement(map, player);
	TurretMonster::updateProjectiles(map, player);
	BasicMonster::refreshTextureIfNecessary();
}

void DistanceMonster::render(Shader* shader)
{
	BasicMonster::render(shader);
	TurretMonster::render(shader);
}

void DistanceMonster::startOnlineMovement(glm::vec3 position)
{

}

void DistanceMonster::action(int actionId, int xid, glm::vec3 vector)
{
	switch (actionId)
	{
		default: break;
	}
}

Monster* DistanceMonster::getCopy(Monster* copy)
{
	// Create new monster if necessary.
	if (copy == nullptr) copy = new DistanceMonster(nullptr, nullptr, nullptr);

	return Monster::getCopy(copy);
}