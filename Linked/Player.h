#pragma once
#include "Entity.h"
#include "Creature.h"
#include "Skill.h"
#include "Equipment.h"

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

class HPBar;

class Player : public Entity, public Creature
{
public:
	Player(Transform* transform, Mesh* mesh, Texture* texture);
	~Player();

	/* NAME */
	std::string getName();
	void setName(std::string name);

	bool isDead();

	void update();
	void input();

	/* HP */
	unsigned int getHp();
	void setHp(unsigned int hp);
	void doDamage(unsigned int damage);
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
	std::vector<Skill>* getSkills();
	Skill* getSkillOfSlot(SkillSlot slot);
	bool addNewSkill(Skill skill);

	/* EQUIPMENTS */
	std::vector<Equipment>* getEquipments();
	Equipment* getEquipmentOfClass(EquipmentClass equipmentClass);
	Equipment* addNewEquipment(Equipment equipment);

	/* HP BAR*/
	HPBar* getHPBar();
private:
	HPBar* hpBar;
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
	std::vector<Skill>* skills;
	std::vector<Equipment>* equipments;
};

