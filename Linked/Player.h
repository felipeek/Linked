#pragma once
#include "Entity.h"
#include "Creature.h"
#include "Skill.h"
#include "Equipment.h"
#include "Movement.h"
#include "PlayerAI.h"
#include "Configuration.h"

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

#define PLAYER_FOG_OF_WAR_RADIUS 30

#define PLAYER_RECEIVE_DAMAGE_DELAY 0.3f
#define PLAYER_TEXTURE_CHANGE_DELAY 0.2f

#define SQRT2 1.414213562373

class HPBar;
class RangeAttack;
class Map;
class Projectile;

enum PlayerType
{
	LOCAL,
	NETWORK
};

class Player : public Entity, public Creature
{
public:
	Player(Transform* transform, Mesh* mesh, Texture* texture, std::vector<Monster*>* monsters, Map* map);
	~Player();

	bool isFogOfWar(glm::vec3 position);

	/* METHODS RELATED TO PLAYER ATTRIBUTES */
	std::string getName();
	void setName(std::string name);
	bool isAlive();
	bool isLocalPlayer();
	void setLocalPlayer(bool localPlayer);
	void doDamage(unsigned int damage);
	unsigned int getHp();
	void setHp(unsigned int hp);
	unsigned int getTotalMaximumHp();
	unsigned int getTotalAttack();
	unsigned int getTotalDefense();
	unsigned int getTotalMagicalPower();
	unsigned int getTotalSpeed();
	unsigned int getTotalAttackSpeed();
	std::vector<Skill*> getSkills();
	Skill* getSkillOfSlot(SkillSlot slot);
	bool addNewSkill(Skill* skill);
	bool isPlayerUsingASkill();
	bool isPlayerUsingSkillOfSlot(SkillSlot slot);
	std::vector<Equipment*> getEquipments();
	Equipment* getEquipmentOfClass(EquipmentClass equipmentClass);
	Equipment* addNewEquipment(Equipment* equipment);
	HPBar* getHPBar();
	RangeAttack* getRangeAttack();

#ifdef SINGLEPLAYER
	void healHp(unsigned int healingAmount);
	void restoreHpToMaximum();
	unsigned int getMaximumHpBasis();
	void setMaximumHpBasis(unsigned int maximumHpBasis);
	unsigned int getAttackBasis();
	void setAttackBasis(unsigned int attackBasis);
	unsigned int getDefenseBasis();
	void setDefenseBasis(unsigned int defenseBasis);
	unsigned int getMagicalPowerBasis();
	void setMagicalPowerBasis(unsigned int magicalPowerBasis);
	unsigned int getSpeedBasis();
	void setSpeedBasis(unsigned int speedBasis);
	unsigned int getAttackSpeedBasis();
	void setAttackSpeedBasis(unsigned int attackSpeedBasis);
#endif

#ifdef MULTIPLAYER
	void setTotalMaximumHp(unsigned int maxHp);
	void setTotalAttack(unsigned int totalAttack);
	void setTotalDefense(unsigned int totalDefense);
	void setTotalMagicalPower(unsigned int totalMagicalPower);
	void setTotalSpeed(unsigned int totalSpeed);
	void setTotalAttackSpeed(unsigned int totalAttackSpeed);
	void startMovementTo(glm::vec3 destination);
	void setType(PlayerType type);
	PlayerType getType();
#endif	

	/* METHODS RELATED TO PLAYER TEXTURE CHANGE */
	void doAttack();
	bool isAttacking();
	bool isMoving();
	void receiveDamage();
	bool isReceivingDamage();

	/* METHODS RELATED TO INPUT, UPDATE AND RENDERING */
	void update(Map* map);
	void input(Map* map);
	void render(Shader* primitiveShader, TextRenderer* textRenderer, Shader* projectileShader);

	/* METHODS RELATED TO NETWORK */
	short getClientId();
	void setClientId(short clientId);
private:
	/* FUNDAMENTAL ATTRIBUTES */
	short clientId;
	bool localPlayer;
	std::string name;
	unsigned int hp;
	std::vector<Skill*> skills;
	std::vector<Equipment*> equipments;
	std::vector<Projectile*> attacks;
	HPBar* hpBar;
	RangeAttack* rangeAttack;

#ifdef SINGLEPLAYER
	unsigned int maximumHpBasis;
	unsigned int attackBasis;
	unsigned int defenseBasis;
	unsigned int magicalPowerBasis;
	unsigned int speedBasis;
	unsigned int attackSpeedBasis;
#endif

#ifdef MULTIPLAYER
	unsigned int maximumHp;
	unsigned int attack;
	unsigned int defense;
	unsigned int magicalPower;
	unsigned int speed;
	unsigned int attackSpeed;
	PlayerType type;
#endif

	/* TEXTURE-RELATED ATTRIBUTES */
	bool attacking;
	bool receivingDamage;
	bool moving;
	double lastAttackTime = 0;
	double lastReceivedDamageTime = 0;
	MovementDirection currentDirection;
	MovementDirection lastDirection;
	double textureChangeTime = 0;
	unsigned int lastIndexTexture;
	bool lastIsAttacking = false;
	bool lastIsReceivingDamage = false;
	bool lastIsDead = false;
	bool lastIsMoving = false;

	/* TEXTURE-RELATED FUNCTIONS */
	void refreshTexture();
	void changeTextureBasedOnDirection(MovementDirection direction, unsigned int initialTextureIndex, unsigned int finalTextureIndex);

	/* MOVEMENT ATTRIBUTES */
#ifdef MULTIPLAYER
	PlayerAI* ai;
	glm::vec3 destination;
	bool isMovingTo;
#endif

	/* MOVEMENT FUNCTIONS */
#ifdef MULTIPLAYER
	void updateMovement(Map* map);
#endif
	bool checkIfPlayerIsStillOnTheSameMapPosition(glm::vec3 currentPosition, glm::vec3 nextPosition);
	glm::vec3 getDeltaVectorToDirection(MovementDirection direction);
};