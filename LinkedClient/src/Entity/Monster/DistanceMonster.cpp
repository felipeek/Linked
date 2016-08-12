#include "DistanceMonster.h"
#include "Projectile.h"
#include "Player.h"
#include "LinkedTime.h"
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

void DistanceMonster::update(Map* map, Player* player, std::vector<Monster*>* monsters)
{
	Monster::update(map, player, monsters);
	BasicMonster::updateMovement(map, player);
	TurretMonster::updateProjectiles(map, player);
	BasicMonster::refreshTextureIfNecessary();
}

void DistanceMonster::render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer)
{
	BasicMonster::render(primitiveShader, skillShader, textRenderer);
	TurretMonster::render(primitiveShader, skillShader, textRenderer);
}

void DistanceMonster::startOnlineMovement(glm::vec3 position)
{
	BasicMonster::startOnlineMovement(position);
}

void DistanceMonster::action(int actionId, int xid, glm::vec3 vector)
{
	BasicMonster::action(actionId, xid, vector);
	TurretMonster::action(actionId, xid, vector);
}

Monster* DistanceMonster::getCopy(Monster* copy)
{
	// Create new monster if necessary.
	if (copy == nullptr) copy = new DistanceMonster(nullptr, nullptr, nullptr);

	return Monster::getCopy(copy);
}