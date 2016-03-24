#pragma once
#include "Monster.h"
#include "Movement.h"

#define TEXTURE_CHANGE_TIME_FACTOR 0.2f

enum class BasicMonsterActiveTexture
{
	DEAD,
	ATTACKING,
	RECEIVING_DAMAGE,
	MOVING,
	STANDING
};

class BasicMonster : virtual public Monster
{
public:
	BasicMonster(Transform* transform, Mesh* mesh, Texture* texture);
	~BasicMonster();

	virtual void update(Map* map, Player* player);
	virtual void render(Shader* shader);
	virtual void startOnlineMovement(glm::vec3 position);
	virtual void action(int actionId, int xid, glm::vec3 vector);

	/* COPY */
	virtual Monster* getCopy(Monster* copy);
protected:
	void refreshTextureIfNecessary();
	void updateMovement(Map* map, Player* player);

private:
	double lastTimeTextureWasRefreshed;
	BasicMonsterActiveTexture activeTexture;
	MovementDirection activeTextureDirection;
	int timeStopped = 0;
	void animateActiveTexture();

	void moveOnline(Map* map);
	void moveToAttackPlayer(Map* map, Player* player);
	void moveRandomly(Map* map, Player* player);

	bool movingRandomly = false;
	bool movingToAttackPlayer = false;
	bool movingOnline = false;

	MovementDefinition directedMovement;

	bool isIntegerOnRange(int integer, int begin, int end);
	void forceMonsterToLookAtPlayer(glm::vec3 playerPosition);
};