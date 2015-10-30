#pragma once

#include <vector>
#include <string>
#include "Skill.h"
#include "Equipment.h"
#include "WorldObject.h"
#include "Creature.h"

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

#define PLAYER_FOG_OF_WAR_RADIUS 40

class Projectile;
class Monster;
class Map;

class Player : public WorldObject, public Creature
{
public:
	Player(glm::vec3 initialPosition, std::vector<Monster*>* monsters, Map* map);
	~Player();

	Player* getLink();
	void setLink(Player* link);

	/* NETWORK */
	unsigned short getClientId();
	void setClientId(unsigned short clientId);
	bool didAttributesChanged();
	void resetAttributesChanged();
	bool isFogOfWar(glm::vec3 position);

	/* NAME */
	std::string getName();
	void setName(std::string name);

	/* COMBAT */
	bool isAlive();
	void doDamage(unsigned int damage);

	/* HP */
	unsigned int getHp();
	void setHp(unsigned int hp);
	void healHp(unsigned int healingAmount);
	void restoreHpToMaximum();
	unsigned int getMaximumHpBasis();
	void setMaximumHpBasis(unsigned int maximumHpBasis);
	unsigned int getTotalMaximumHp();

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

	/* UPDATE */
	void update();

private:
	/* PLAYER ATTRIBUTES/STATUS */
	Player* link;
	unsigned short clientId;
	bool attributesChanged;
	std::string name;
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
	std::vector<Projectile*> attacks;
};