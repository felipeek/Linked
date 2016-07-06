#pragma once
#include "Monster.h"
#include "Movement.h"

class BasicMonster : public virtual Monster
{
public:
	BasicMonster();
	~BasicMonster();

	virtual void update(Map* map, std::vector<Player*>* players, std::vector<Monster*>* monsters);

	/* COPY */
	virtual Monster* getCopy(Monster* copy);

	/* NETWORK MOVEMENT */
	virtual bool mustUpdateDestinationToClients();
	virtual glm::vec3 getDestination();

protected:
	void updateMovement(Map* map, std::vector<Player*>* players);

private:
	void moveToAttackPlayer(Map* map, Player* player);
	void moveRandomly(Map* map);

	bool movingRandomly = false;
	bool movingToAttackPlayer = false;
	MovementDefinition directedMovement;
	MovementDefinition oldDirectedMovement;
};