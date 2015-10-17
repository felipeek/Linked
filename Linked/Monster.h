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

#define TEXTURE_CHANGE_DELAY 0.15f
#define ASPD_FACTOR 10
#define DEATH_TIME 5.0f
#define RECEIVE_DAMAGE_DELAY 0.3f

// If MONSTER_FIRST_DIRECTION is changed,
// MONSTER_FIRST_INDEX_TEXTURE must also be changed.
// Plus, the texture index must be an index of a texture which
// direction is the same as defined on MONSTER_FIRST_DIRECTION
#define MONSTER_FIRST_DIRECTION BOTTOM
#define MONSTER_FIRST_INDEX_TEXTURE 20

class Player;
class MonsterAI;
class Map;
class MovementDefinition;
enum MovementDirection;

class Monster : public Entity, public Creature
{
public:
	Monster(Transform* transform, Mesh* mesh, Texture* texture);
	~Monster();

	/* BASIC ATTRIBUTES */
	int getId();
	void setId(int id);
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
	bool shouldRender();
	void setShouldRender(bool shouldRender);
	bool isMoving();
	bool isOnScreen();

	/* COMBAT */
	void doDamage(unsigned int damage);
	void attackCreature(Creature* creature);
	
	/* MOVEMENT */
	void startMovementTo(glm::vec3 destination);
	
	/* UPDATE */
	void update(Map* map, Player* player);
	
private:
	MonsterAI* ai;

	/* BASIC ATTRIBUTES */
	int id;
	std::string name;
	bool bRender;
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

	/* TEXTURE MANAGEMENT AUXILIAR FUNCTIONS */
	void changeTexture(MovementDirection direction);
	void changeTextureBasedOnDirection(MovementDirection direction, unsigned int initialTextureIndex, unsigned int finalTextureIndex);
	int decodeMonsterIndex(int index);

	/* TEXTURE MANAGEMENT AUXILIAR VARIABLES */
	MovementDirection currentDirection;
	double textureChangeTime = 0;
	unsigned int lastIndexTexture;
	bool lastIsAttacking = false;
	bool lastIsReceivingDamage = false;
	bool lastIsDead = false;
	bool lastIsMoving = false;

	/* MOVEMENT AUXILIAR FUNCTIONS */
	MovementDefinition moveTo(Entity* entity, Map* map); // single player
	MovementDefinition moveRandomly(Map* map); // single player
	bool hasReachedEntity(Entity* entity); // single player
	MovementDefinition updateMovement(Map* map); // multiplayer

	/* MOVEMENT AUXILIAR VARIABLES */
	glm::vec3 destination;
	bool isMovingTo;
};