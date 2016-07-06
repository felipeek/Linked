#include "BomberMonster.h"
#include "MonsterExplosionSkill.h"
#include "MonsterEndureSkill.h"
#include "Projectile.h"
#include "Player.h"
#include "LinkedTime.h"
#include "Audio.h"
#include "Game.h"
#include "MonsterAI.h"

BomberMonster::BomberMonster(Transform* transform, Mesh* mesh, Texture* texture) : Monster(transform, mesh, texture), BasicMonster(transform, mesh, texture)
{
	this->explosionSkill = new MonsterExplosionSkill(SkillOwner::MONSTER);
	this->explosionSkill->setEntity(this);
	this->endureSkill = new MonsterEndureSkill(SkillOwner::MONSTER);
	this->endureSkill->setEntity(this);
}

BomberMonster::~BomberMonster()
{
	delete this->explosionSkill;
	delete this->endureSkill;
}

void BomberMonster::attackCreature(Creature* creature)
{
	if (Game::multiplayer)
	{
		this->attack();
		if (!this->explosionSkill->isActive())
			this->explode();
	}
}

void BomberMonster::update(Map* map, Player* player, std::vector<Monster*>* monsters)
{
	this->explosionSkill->update(monsters, nullptr, player);
	this->endureSkill->update(monsters, nullptr, player);
	Monster::update(map, player, monsters);
	BasicMonster::updateMovement(map, player);
	BasicMonster::refreshTextureIfNecessary();
}

void BomberMonster::doDamage(unsigned int damage)
{
	if (!this->endureSkill->isActive())
	{
		int randomNumber = rand() % 100;
		if (ENDURE_CHANCE > randomNumber)
			this->endureSkill->execute(MovementDirection::BOTTOM, glm::vec3(0, 0, 0), 0);
	}

	BasicMonster::doDamage(damage);

	if (!this->isAlive())
		if (!this->explosionSkill->isActive())
			this->explode();
}

void BomberMonster::render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer)
{
	this->explosionSkill->render(primitiveShader, skillShader, textRenderer);
	this->endureSkill->render(primitiveShader, skillShader, textRenderer);
	BasicMonster::render(primitiveShader, skillShader, textRenderer);
}

void BomberMonster::startOnlineMovement(glm::vec3 position)
{
	BasicMonster::startOnlineMovement(position);
}

void BomberMonster::action(int actionId, int xid, glm::vec3 vector)
{
	if (actionId == 2)
		if (!this->explosionSkill->isActive())
			this->explode();
	BasicMonster::action(actionId, xid, vector);
}

Monster* BomberMonster::getCopy(Monster* copy)
{
	// Create new monster if necessary.
	if (copy == nullptr) copy = new BomberMonster(nullptr, nullptr, nullptr);

	return Monster::getCopy(copy);
}

void BomberMonster::explode()
{
	this->doDamage(this->getHp());
	this->explosionSkill->execute(MovementDirection::BOTTOM, this->getTransform()->getPosition(), 0);
}