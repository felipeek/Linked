#pragma once
#include "Entity.h"
#include "Creature.h"
#include "Skill.h"
#include "Equipment.h"
#include "Movement.h"
#include "PlayerAI.h"

#include <vector>
#include <string>

#define PLAYER_MAXIMUM_SKILLS 4
#define PLAYER_DEFAULT_NAME "unnamed"
#define PLAYER_DEFAULT_HP 100
#define PLAYER_DEFAULT_MAX_HP_BASIS 100
#define PLAYER_DEFAULT_LIVES 3
#define PLAYER_DEFAULT_MAX_LIVES_BASIS 3
#define PLAYER_DEFAULT_ATTACK_BASIS 10
#define PLAYER_DEFAULT_DEFENSE_BASIS 10
#define PLAYER_DEFAULT_MAGICAL_POWER_BASIS 10
#define PLAYER_DEFAULT_SPEED_BASIS 10
#define PLAYER_DEFAULT_ATTACK_SPEED_BASIS 10

#define PLAYER_RECEIVE_DAMAGE_DELAY 0.3f
#define PLAYER_TEXTURE_CHANGE_DELAY 0.2f

#define SQRT2 1.414213562373

class HPBar;
class RangeAttack;
class Map;

enum PlayerType
{
	LOCAL,
	NETWORK
};

class Player : public Entity, public Creature
{
public:
	Player(Transform* transform, Mesh* mesh, Texture* texture, RangeAttack* rangeAttack);
	Player(Transform* transform, Mesh* mesh, Texture* texture);
	~Player();

	/* NAME */
	std::string getName();
	void setName(std::string name);

	/* COMBAT */
	bool isAlive();
	bool isAttacking();
	bool isReceivingDamage();
	void attack();
	void doDamage(unsigned int damage);

	/* HP */
	unsigned int getHp();
	void setHp(unsigned int hp);
	void healHp(unsigned int healingAmount);
	void restoreHpToMaximum();
	unsigned int getMaximumHpBasis();
	void setMaximumHpBasis(unsigned int maximumHpBasis);
	unsigned int getTotalMaximumHp();

	/* LIVES */
	unsigned int getLives();
	void setLives(unsigned int lives);
	void removeLive();
	void restoreAllLives();
	void restoreLive();
	unsigned int getMaximumLivesBasis();
	void setMaximumLivesBasis(unsigned int maximumLiveBasis);
	unsigned int getTotalMaximumLives();

	/* ATTACK */
	unsigned int getAttackBasis();
	void setAttackBasis(unsigned int attackBasis);
	unsigned int getTotalAttack();

	/* DEFENSE */
	unsigned int getDefenseBasis();
	void setDefenseBasis(unsigned int defenseBasis);
	unsigned int getTotalDefense();

	/* MAGICAL POWER */
	unsigned int getMagicalPowerBasis();
	void setMagicalPowerBasis(unsigned int magicalPowerBasis);
	unsigned int getTotalMagicalPower();

	/* SPEED */
	unsigned int getSpeedBasis();
	void setSpeedBasis(unsigned int speedBasis);
	unsigned int getTotalSpeed();

	/* ASPD */
	unsigned int getAttackSpeedBasis();
	void setAttackSpeedBasis(unsigned int attackSpeedBasis);
	unsigned int getTotalAttackSpeed();

	/* SKILLS */
	std::vector<Skill*> getSkills();
	Skill* getSkillOfSlot(SkillSlot slot);
	bool addNewSkill(Skill* skill);
	bool isPlayerUsingASkill();
	bool isPlayerUsingSkillOfSlot(SkillSlot slot);

	/* EQUIPMENTS */
	std::vector<Equipment*> getEquipments();
	Equipment* getEquipmentOfClass(EquipmentClass equipmentClass);
	Equipment* addNewEquipment(Equipment* equipment);

	/* HP BAR */
	HPBar* getHPBar();

	/* RANGE ATTACK */
	RangeAttack* getRangeAttack();
	void setRangeAttack(RangeAttack* rangeAttack);

	/* INPUT, UPDATE & RENDERING */
	void update(Map* map);
	void input(Map* map);
	void render(Shader* primitiveShader, TextRenderer* textRenderer);

	/* MOVEMENT */
	void startMovementTo(glm::vec3 destination);

	/* NETWORK */
	void setType(PlayerType type);
	PlayerType getType();

private:
	HPBar* hpBar;
	RangeAttack* rangeAttack;

	/* PLAYER ATTRIBUTES/STATUS */
	std::string name;
	bool attacking;
	bool receivingDamage;
	unsigned int hp;
	unsigned int maximumHpBasis;
	unsigned int lives;
	unsigned int maximumLivesBasis;
	unsigned int attackBasis;
	unsigned int defenseBasis;
	unsigned int magicalPowerBasis;
	unsigned int speedBasis;
	unsigned int attackSpeedBasis;
	std::vector<Skill*> skills;
	std::vector<Equipment*> equipments;

	/* TIME-BASED ATTRIBUTES AUXILIAR VARIABLES */
	double lastAttackTime = 0;
	double lastReceivedDamageTime = 0;

	/* MOVEMENT AUXILIAR FUNCTIONS */
	bool checkIfPlayerIsStillOnTheSameMapPosition(glm::vec3 currentPosition, glm::vec3 nextPosition);
	glm::vec3 getDeltaVectorToDirection(MovementDirection direction);

	/* TEXTURE MANAGEMENT AUXILIAR FUNCTIONS */
	void refreshTexture();
	void changeTextureBasedOnDirection(MovementDirection direction, unsigned int initialTextureIndex, unsigned int finalTextureIndex);

	/* TEXTURE MANAGEMENT AUXILIAR VARIABLES */
	MovementDirection currentDirection;
	MovementDirection lastDirection;
	double textureChangeTime = 0;
	unsigned int lastIndexTexture;
	bool lastIsAttacking = false;
	bool lastIsReceivingDamage = false;
	bool lastIsDead = false;

	/* MOVEMENT AUXILIAR VARIABLES */
	PlayerAI* ai;
	glm::vec3 destination;
	bool isMovingTo;

	/* MOVEMENT AUXILIAR FUNCTIONS */
	void updateMovement(Map* map);

	/* NETWORK VARIABLES */
	PlayerType type;
};