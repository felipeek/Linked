#pragma once
#include "Monster.h"
#include "Movement.h"

class BasicMonster : public Monster
{
public:
	BasicMonster();
	~BasicMonster();

	virtual void update(Map* map, std::vector<Player*>* players);

	/* COPY */
	virtual Monster* getCopy(Monster* copy);
private:
	void moveToAttackPlayer(Map* map, Player* player);
	void moveRandomly(Map* map, Player* player);

	bool movingRandomly = false;
	bool movingToAttackPlayer = false;
	MovementDefinition directedMovement;
};