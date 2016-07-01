#include "BomberMonster.h"
#include "Projectile.h"
#include "Player.h"
#include "LinkedTime.h"
#include "Audio.h"
#include "Game.h"
#include "MonsterAI.h"

BomberMonster::BomberMonster(Transform* transform, Mesh* mesh, Texture* texture) : Monster(transform, mesh, texture), BasicMonster(transform, mesh, texture)
{
}

BomberMonster::~BomberMonster()
{
}

void BomberMonster::attackCreature(Creature* creature)
{
	this->attack();
	this->doDamage(this->getHp());
}

void BomberMonster::update(Map* map, Player* player)
{
	Monster::update(map, player);
	BasicMonster::updateMovement(map, player);
	BasicMonster::refreshTextureIfNecessary();
}

void BomberMonster::render(Shader* shader)
{
	BasicMonster::render(shader);
}

void BomberMonster::startOnlineMovement(glm::vec3 position)
{
	BasicMonster::startOnlineMovement(position);
}

void BomberMonster::action(int actionId, int xid, glm::vec3 vector)
{
	BasicMonster::action(actionId, xid, vector);
}

Monster* BomberMonster::getCopy(Monster* copy)
{
	// Create new monster if necessary.
	if (copy == nullptr) copy = new BomberMonster(nullptr, nullptr, nullptr);

	return Monster::getCopy(copy);
}