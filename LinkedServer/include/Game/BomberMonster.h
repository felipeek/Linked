#pragma once
#include "BasicMonster.h"
#include "Skill.h"

#define ENDURE_CHANCE 20

class BomberMonster : public BasicMonster
{
public:
	BomberMonster();
	~BomberMonster();

	virtual void update(Map* map, std::vector<Player*>* players, std::vector<Monster*>* monsters);

	/* COPY */
	virtual Monster* getCopy(Monster* copy);

	virtual void attackCreature(Creature* creature);

	virtual void doDamage(unsigned int damage);

private:
	Skill* explosionSkill;
	Skill* endureSkill;
	void explode();
};