#pragma once
#include "Entity.h"
#include "Creature.h"
class Player;

#define MONSTER_DEFAULT_NAME "unnamed"
#define MONSTER_DEFAULT_HP 10
#define MONSTER_DEFAULT_TOTAL_MAX_HP 10
#define MONSTER_DEFAULT_TOTAL_ATTACK 10
#define MONSTER_DEFAULT_TOTAL_DEFENSE 10
#define MONSTER_DEFAULT_TOTAL_MAGICAL_POWER 10
#define MONSTER_DEFAULT_TOTAL_SPEED 10
#define MONSTER_DEFAULT_TOTAL_ATTACK_SPEED 10

#define TEXTURE_CHANGE_DELAY 0.2f
#define ASPD_FACTOR 10
#define DEATH_TIME 5.0f

// If MONSTER_FIRST_DIRECTION is changed,
// MONSTER_FIRST_INDEX_TEXTURE must also be changed.
// Plus, the texture index must be an index of a texture which
// direction is the same as defined on MONSTER_FIRST_DIRECTION
#define MONSTER_FIRST_DIRECTION BOTTOM
#define MONSTER_FIRST_INDEX_TEXTURE 20

class MonsterAI;
class Map;
class MovementDefinition;
enum MovementDirection;

class Monster : public Entity, public Creature
{
public:
	Monster(Transform* transform, Mesh* mesh, Texture* texture);
	~Monster();
	std::string getName();
	void setName(std::string name);
	bool isAlive();
	void killMonster(int index);
	bool checkIfMustBeDeleted();
	bool canBeDeleted();
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
	void update(Map* map, Player* player);
private:
	std::string name;
	bool alive;
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
	MonsterAI* ai;
	MovementDirection currentDirection;
	void changeTexture(MovementDirection direction);
	void changeTextureBasedOnDirection(MovementDirection direction, unsigned int initialTextureIndex, unsigned int finalTextureIndex);

	// auxiliar variable to help to change textures on the right time
	double textureChangeTime = 0;
	unsigned int lastIndexTexture;

	MovementDefinition moveTo(Entity* entity, Map* map);
	MovementDefinition moveAway(Entity* entity, Map* map);
	MovementDefinition moveRandomly(Map* map);
	bool hasReachedEntity(Entity* entity);

	double killTime = 0;
	double lastAttackTime = 0;
};