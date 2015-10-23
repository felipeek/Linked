#pragma once
#include "WorldObject.h"
#include "Creature.h"
#include <iostream>
#include <vector>

#define MONSTER_DEFAULT_NAME "unnamed"
#define MONSTER_DEFAULT_HP 10
#define MONSTER_DEFAULT_TOTAL_MAX_HP 10
#define MONSTER_DEFAULT_TOTAL_ATTACK 10
#define MONSTER_DEFAULT_TOTAL_DEFENSE 10
#define MONSTER_DEFAULT_TOTAL_MAGICAL_POWER 10
#define MONSTER_DEFAULT_TOTAL_SPEED 10
#define MONSTER_DEFAULT_TOTAL_ATTACK_SPEED 10
#define MONSTER_DEFAULT_TOTAL_RANGE 10

#define FRAMETIME 1.0f/60.0f
#define ASPD_FACTOR 10
#define RECEIVE_DAMAGE_DELAY 0.3f

#define DEATH_TIME 5.0f

class Player;
class MonsterAI;
class Map;
class MovementDefinition;
enum MovementDirection;

class Monster : public WorldObject, public Creature
{
public:
	Monster();
	~Monster();
	std::string getName();
	void setName(std::string name);
	bool isAlive();
	bool isAttacking();
	bool isReceivingDamage();
	bool isMoving();
	bool wasUpdatedToClient();
	void setUpdatedToClient(bool updatedToClient);
	void attack();
	void receiveDamage();
	void killMonster();
	bool isOnScreen();
	unsigned int getHp();
	void setHp(unsigned int hp);
	unsigned int getTotalMaximumHp();
	void setTotalMaximumHp(unsigned int totalMaximumHp);
	void doDamage(unsigned int damage);
	unsigned int getTotalAttack();
	void setTotalAttack(unsigned int totalAttack);
	unsigned int getTotalDefense();
	void setTotalDefense(unsigned int totalDefense);
	unsigned int getTotalSpeed();
	void setTotalSpeed(unsigned int totalSpeed);
	unsigned int getTotalRange();
	void setTotalRange(unsigned int totalRange);
	unsigned int getTotalCollisionRange();
	void setTotalCollisionRange(unsigned int totalCollisionRange);
	unsigned int getTotalAttackSpeed();
	void setTotalAttackSpeed(unsigned int totalAttackSpeed);
	unsigned int getTotalMagicalPower();
	void setTotalMagicalPower(unsigned int magicalPower);
	int getMapColorRed();
	void setMapColorRed(int red);
	int getMapColorGreen();
	void setMapColorGreen(int green);
	int getMapColorBlue();
	void setMapColorBlue(int blue);
	glm::vec3 getMapColor();
	void setMapColor(glm::vec3 mapColor);
	void attackCreature(Creature* creature);

	void update(Map* map, std::vector<Player*>* players);
private:
	MonsterAI* ai;

	/* MONSTER ATTRIBUTES/STATUS */
	std::string name;
	bool updatedToClient;
	bool alive;
	bool attacking;
	bool receivingDamage;
	bool moving;
	unsigned int hp;
	unsigned int totalMaximumHp;
	unsigned int totalAttack;
	unsigned int totalDefense;
	unsigned int totalMagicalPower;
	unsigned int totalSpeed;
	unsigned int totalRange;
	unsigned int totalAttackSpeed;
	unsigned int totalCollisionRange;
	glm::vec3 mapColor;

	/* TIME-BASED ATTRIBUTES AUXILIAR VARIABLES */
	double killTime = 0;
	double lastAttackTime = 0;
	double lastReceivedDamageTime = 0;

	/* MOVEMENT AUXILIAR FUNCTIONS */
	MovementDefinition moveTo(WorldObject* worldObject, Map* map);
	MovementDefinition moveRandomly(Map* map);
	bool hasReachedEntity(WorldObject* worldObject);
	Player* getClosestAlivePlayer(std::vector<Player*>* players);

	bool isKnockedBack;
};