#pragma once
#include "BasicMonster.h"

#define ENDURE_CHANCE 20

class Skill;

class BomberMonster : public BasicMonster
{
public:
	BomberMonster(Transform* transform, Mesh* mesh, Texture* texture);
	~BomberMonster();

	/* UPDATE & RENDER */
	virtual void update(Map* map, Player* player, std::vector<Monster*>* monsters);
	virtual void render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer);

	/* MOVEMENT */
	virtual void startOnlineMovement(glm::vec3 position);

	virtual void action(int actionId, int xid, glm::vec3 vector);

	virtual void attackCreature(Creature* creature);

	virtual void doDamage(unsigned int damage);

	/* COPY */
	// if the "copy" parameter is NULL, it will allocate the monster
	// if not, it will just copy the attributes to the existing monster.
	virtual Monster* getCopy(Monster* copy);
protected:

private:
	Skill* explosionSkill;
	Skill* endureSkill;
};