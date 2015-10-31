#pragma once
#include "Entity.h"
#include "Creature.h"
#include "Movement.h"

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

// If PLAYER_FIRST_DIRECTION is changed,
// PLAYER_FIRST_INDEX_TEXTURE must also be changed.
// Plus, the texture index must be an index of a texture which
// direction is the same as defined on MONSTER_FIRST_DIRECTION
#define PLAYER_FIRST_DIRECTION BOTTOM
#define PLAYER_FIRST_INDEX_TEXTURE 123

#define SQRT2 1.414213562373

class HPBar;
class RangeAttack;
class Map;
class Projectile;
class Equipment;
class PlayerAI;
class Skill;
class Monster;
class TextRenderer;
enum EquipmentClass;
enum SkillSlot;

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

	/* BASIC ATTRIBUTES */
	Player* getLink();
	void setLink(Player* player);
	short getClientId();	// multiplayer
	void setClientId(short clientId);	// multiplayer
	PlayerType getType();	// multiplayer
	void setType(PlayerType type);	// multiplayer
	std::string getName();
	void setName(std::string name);
	unsigned int getHp();
	void setHp(unsigned int hp);
	void healHp(unsigned int healingAmount);
	void restoreHpToMaximum();
	unsigned int getMaximumHpBasis();	// single player
	void setMaximumHpBasis(unsigned int maximumHpBasis);	// single player
	unsigned int getTotalMaximumHp();
	void setTotalMaximumHp(unsigned int maxHp);	// multiplayer
	unsigned int getAttackBasis();	// single player
	void setAttackBasis(unsigned int attackBasis);	// single player
	unsigned int getTotalAttack();
	void setTotalAttack(unsigned int totalAttack);	// multiplayer
	unsigned int getDefenseBasis();	// single player
	void setDefenseBasis(unsigned int defenseBasis);	// single player
	unsigned int getTotalDefense();
	void setTotalDefense(unsigned int totalDefense);	// multiplayer
	unsigned int getMagicalPowerBasis();	// single player
	void setMagicalPowerBasis(unsigned int magicalPowerBasis);	// single player
	unsigned int getTotalMagicalPower();
	void setTotalMagicalPower(unsigned int totalMagicalPower);	// multiplayer
	unsigned int getSpeedBasis();	// single player
	void setSpeedBasis(unsigned int speedBasis);	// single player
	unsigned int getTotalSpeed();
	void setTotalSpeed(unsigned int totalSpeed);	// multiplayer
	unsigned int getAttackSpeedBasis();	// single player
	void setAttackSpeedBasis(unsigned int attackSpeedBasis);	// single player
	unsigned int getTotalAttackSpeed();
	void setTotalAttackSpeed(unsigned int totalAttackSpeed);	// multiplayer
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

	/* STATUS */
	bool isAlive();
	bool isMoving();
	bool isAttacking();
	void doAttack();
	void receiveDamage();
	bool isReceivingDamage();

	/* COMBAT */
	void doDamage(unsigned int damage);
	
	/* MOVEMENT */
	void startMovementTo(glm::vec3 destination);	// multiplayer

	/* AUXILIAR */
	bool isFogOfWar(glm::vec3 position);

	/* METHODS RELATED TO INPUT, UPDATE AND RENDERING */
	void update(Map* map);
	void input(Map* map);
	void render(Shader* primitiveShader, Shader* skillShader, Shader* worldSkillShader, TextRenderer* textRenderer, Shader* projectileShader);

	/* HPBAR RELATED */
	void hpBarRenderOptions(bool shouldRender);
	bool hpBar_shouldRender;
private:
	/* FUNDAMENTAL ATTRIBUTES */
	Player* link;
	short clientId;
	std::string name;
	unsigned int hp;
	unsigned int maximumHpBasis;	// single player
	unsigned int maximumHp;	// multiplayer
	unsigned int attackBasis;	// single player
	unsigned int attack;	// multiplayer
	unsigned int defenseBasis;	// single player
	unsigned int defense;	// multiplayer
	unsigned int magicalPowerBasis;	// single player
	unsigned int magicalPower;	// multiplayer
	unsigned int speedBasis;	// single player
	unsigned int speed;	// multiplayer
	unsigned int attackSpeedBasis;	// single player
	unsigned int attackSpeed;	// multiplayer
	PlayerType type;
	std::vector<Skill*> skills;
	std::vector<Equipment*> equipments;
	std::vector<Projectile*> attacks;
	HPBar* hpBar;
	RangeAttack* rangeAttack;

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

	/* TEXTURE-RELATED METHODS */
	void refreshTexture();
	void changeTextureBasedOnDirection(MovementDirection direction, unsigned int initialTextureIndex, unsigned int finalTextureIndex);

	/* MOVEMENT ATTRIBUTES */
	PlayerAI* ai;	// multiplayer
	glm::vec3 destination;	// multiplayer
	bool isMovingTo;	// multiplayer

	/* MOVEMENT METHODS */
	void updateMovement(Map* map);	// multiplater

	/* AUXILIAR METHODS */
	bool checkIfPlayerIsStillOnTheSameMapPosition(glm::vec3 currentPosition, glm::vec3 nextPosition);
	glm::vec3 getDeltaVectorToDirection(MovementDirection direction);
};