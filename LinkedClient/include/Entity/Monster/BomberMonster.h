#pragma once
#include "BasicMonster.h"

class BomberMonster : public BasicMonster
{
public:
	BomberMonster(Transform* transform, Mesh* mesh, Texture* texture);
	~BomberMonster();

	/* UPDATE & RENDER */
	virtual void update(Map* map, Player* player);
	virtual void render(Shader* shader);

	/* MOVEMENT */
	virtual void startOnlineMovement(glm::vec3 position);

	virtual void action(int actionId, int xid, glm::vec3 vector);

	virtual void attackCreature(Creature* creature);

	/* COPY */
	// if the "copy" parameter is NULL, it will allocate the monster
	// if not, it will just copy the attributes to the existing monster.
	virtual Monster* getCopy(Monster* copy);
protected:

private:
};