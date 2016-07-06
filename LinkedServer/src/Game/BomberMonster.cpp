#include "BomberMonster.h"
#include "Player.h"
#include "MonsterAI.h"
#include "Map.h"
#include "Game.h"
#include "PacketController.h"
#include "LinkedTime.h"
#include "MonsterExplosionSkill.h"
#include "MonsterEndureSkill.h"

BomberMonster::BomberMonster()
{
	this->explosionSkill = new MonsterExplosionSkill(SkillOwner::MONSTER);
	this->explosionSkill->setWorldObject(this);
	this->endureSkill = new MonsterEndureSkill(SkillOwner::MONSTER);
	this->endureSkill->setWorldObject(this);
}

BomberMonster::~BomberMonster()
{
}

void BomberMonster::attackCreature(Creature* creature)
{
	this->attack();
	if (!this->explosionSkill->isActive())
		this->explode();
}

void BomberMonster::update(Map* map, std::vector<Player*>* players, std::vector<Monster*>* monsters)
{
	this->explosionSkill->update(monsters, players);
	this->endureSkill->update(monsters, players);
	Monster::update(map, players, monsters);
	BasicMonster::updateMovement(map, players);
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

Monster* BomberMonster::getCopy(Monster* copy)
{
	// Create new monster if necessary.
	if (copy == nullptr) copy = new BomberMonster();

	return Monster::getCopy(copy);
}

void BomberMonster::explode()
{
	this->doDamage(this->getHp());
	this->explosionSkill->execute(MovementDirection::BOTTOM, this->getPosition(), 0);
}