#pragma once
#include "Entity.h"
#include "Creature.h"

#define MONSTER_DEFAULT_NAME "unnamed"
#define MONSTER_DEFAULT_HP 10
#define MONSTER_DEFAULT_TOTAL_MAX_HP 10
#define MONSTER_DEFAULT_TOTAL_ATTACK 10
#define MONSTER_DEFAULT_TOTAL_DEFENSE 10
#define MONSTER_DEFAULT_TOTAL_MAGICAL_POWER 10
#define MONSTER_DEFAULT_TOTAL_SPEED 10
#define MONSTER_DEFAULT_TOTAL_ATTACK_SPEED 10
#define MONSTER_DEFAULT_TOTAL_RANGE 10

#define ASPD_FACTOR 25
#define DEATH_TIME 5.0f
#define RECEIVE_DAMAGE_DELAY 0.3f

class Player;
class MonsterAI;
class Map;
enum MovementDirection;

class Monster : public Entity, public Creature
{
public:
	Monster(Transform* transform, Mesh* mesh, Texture* texture);
	virtual ~Monster();

	/* BASIC ATTRIBUTES */
	int getId();
	void setId(int id);
	unsigned int getTextureQuantity();
	void setTextureQuantity(unsigned int textureQuantity);
	std::string getName();
	void setName(std::string name);
	unsigned int getHp();
	void setHp(unsigned int hp);
	unsigned int getTotalMaximumHp();
	void setTotalMaximumHp(unsigned int totalMaximumHp);
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

	/* STATUS */
	bool isAlive();
	void killMonster();
	bool isAttacking();
	void attack();
	bool isReceivingDamage();
	void receiveDamage();
	bool shouldTranslate();
	void setShouldTranslate(bool shouldTranslate);
	bool isMoving();
	void move(MovementDirection direction);
	void stop();
	bool shouldBeDeleted();

	/* COMBAT */
	void doDamage(unsigned int damage);
	void attackCreature(Creature* creature);
	
	/* UPDATE & RENDER */
	virtual void update(Map* map, Player* player);
	virtual void render(Shader* shader);

	/* MOVEMENT */
	virtual void startOnlineMovement(glm::vec3 position);

	/* COPY */
	// if the "copy" parameter is NULL, it will allocate the monster
	// if not, it will just copy the attributes to the existing monster.
	virtual Monster* getCopy(Monster* copy);

protected:
	/* AI */
	MonsterAI* ai;

	/* TEXTURE */
	void changeTextureIndex(int index);
	int getCurrentTextureIndex();

private:
	/* BASIC ATTRIBUTES */
	int id;
	unsigned int textureQuantity;
	std::string name;
	bool bTranslate;
	bool alive = true;
	bool attacking = false;
	bool receivingDamage = false;
	bool moving = false;
	MovementDirection movingDirection;
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

	/* ID */
	static unsigned short NEXT_ID;
};