#pragma once

#include <vector>
#include "Skill.h"
#include "Equipment.h"

enum PlayerStatus{
	NORMAL,
	SLOW,
	INVUNERABLE
};

class Player
{
public:
	Player(unsigned int hpMaximum, unsigned int livesMaximum, std::vector<Skill> &skill, std::vector<Equipment> &equipments, int basisAttack, int basisDefense, float basisRange);
	~Player();

	unsigned int getHp();
	unsigned int getLives();
	PlayerStatus getStatus();
	std::vector<Skill> getSkills();
	Equipment* getEquipment(EquipmentClass equipmentClass);
	void changeStatus(PlayerStatus status);
	void addSkill(Skill skill);
	Equipment addEquipment(Equipment equipment);
private:
	unsigned int hp;
	unsigned int lives;
	int getAttack();
	int getDefense();
	PlayerStatus status;
	std::vector<Skill> skills;
	std::vector<Equipment> equipments;

	unsigned int hpMaximum;
	unsigned int livesMaximum;
	unsigned int basisAttack;
	unsigned int basisDefense;
	unsigned int basisRange;
};

