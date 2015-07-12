#pragma once

#include <vector>
#include "Skill.h"
#include "Equipment.h"
#include <string>
#define MAXIMUM_SKILLS 4

class Player
{
public:
	Player();
	~Player();

	/* NAME */
	std::string getName();
	void setName(std::string name);

	/* HP */
	unsigned int getHp();
	void receiveDamage(unsigned int damage);
	void healHp(unsigned int healingAmount);
	void restoreHpToMaximum();
	unsigned int getMaximumHpBasis();
	void setMaximumHpBasis(unsigned int maximumHpBasis);
	unsigned int getTotalMaximumHp();

	/* LIVES */
	unsigned int getLives();
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

	/* SKILLS */
	std::vector<Skill>* getSkills();
	Skill* getSkillOfSlot(SkillSlot slot);
	bool addNewSkill(Skill skill);

	/* EQUIPMENTS */
	std::vector<Equipment>* getEquipments();
	Equipment* getEquipmentOfClass(EquipmentClass equipmentClass);
	Equipment* addNewEquipment(Equipment equipment);

private:
	std::string name;
	unsigned int hp;
	unsigned int maximumHpBasis;
	unsigned int lives;
	unsigned int maximumLivesBasis;
	unsigned int attackBasis;
	unsigned int defenseBasis;
	unsigned int magicalPowerBasis;
	std::vector<Skill>* skills;
	std::vector<Equipment>* equipments;
};

