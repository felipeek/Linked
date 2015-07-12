#include "Player.h"
#define DEFAULT_NAME "unnamed"
#define DEFAULT_HP 0
#define DEFAULT_MAX_HP_BASIS 0
#define DEFAULT_LIVES 0
#define DEFAULT_MAX_LIVES_BASIS 0
#define DEFAULT_ATTACK_BASIS 0
#define DEFAULT_DEFENSE_BASIS 0
#define DEFAULT_MAGICAL_POWER_BASIS 0

Player::Player()
{
	name = DEFAULT_NAME;
	hp = DEFAULT_HP;
	maximumHpBasis = DEFAULT_MAX_HP_BASIS;
	lives = DEFAULT_LIVES;
	maximumLivesBasis = DEFAULT_MAX_LIVES_BASIS;
	attackBasis = DEFAULT_ATTACK_BASIS;
	defenseBasis = DEFAULT_DEFENSE_BASIS;
	magicalPowerBasis = DEFAULT_MAGICAL_POWER_BASIS;
	skills = new std::vector<Skill>();
	equipments = new std::vector<Equipment>();
}

Player::~Player()
{
	delete skills;
	delete equipments;
}

/* HP */

std::string Player::getName(){
	return name;
}
void Player::setName(std::string name){
	this->name = name;
}
unsigned int Player::getHp(){
	return hp;
}
void Player::receiveDamage(unsigned int damage){
	if (damage > hp)
		hp = 0;
	else
		hp = hp - damage;
}
void Player::healHp(unsigned int healingAmount){
	unsigned int maximumHp = getTotalMaximumHp();
	if ((healingAmount + hp) > maximumHp)
		hp = maximumHp;
	else
		hp = healingAmount + hp;
}
void Player::restoreHpToMaximum(){
	hp = getTotalMaximumHp();
}
unsigned int Player::getMaximumHpBasis(){
	return maximumHpBasis;
}
void Player::setMaximumHpBasis(unsigned int maximumHpBasis){
	this->maximumHpBasis = maximumHpBasis;
}
unsigned int Player::getTotalMaximumHp(){
	// TO DO
	return 0;
}

/* LIVES */
unsigned int Player::getLives(){
	return lives;
}
void Player::removeLive(){
	if (lives > 0)
		lives--;
}
void Player::restoreAllLives(){
	lives = getTotalMaximumLives();
}
void Player::restoreLive(){
	if (lives < getTotalMaximumLives())
		lives++;
}
unsigned int Player::getMaximumLivesBasis(){
	return maximumLivesBasis;
}
unsigned int Player::getTotalMaximumLives(){
	// TO DO
	return 0;
}

/* ATTACK */
unsigned int Player::getAttackBasis(){
	return attackBasis;
}
void Player::setAttackBasis(unsigned int attackBasis){
	this->attackBasis = attackBasis;
}
unsigned int getTotalAttack(){
	// TO DO
	return 0;
}

/* DEFENSE */
unsigned int Player::getDefenseBasis(){
	return defenseBasis;
}
void Player::setDefenseBasis(unsigned int defenseBasis){
	this->defenseBasis = defenseBasis;
}
unsigned int Player::getTotalDefense(){
	// TO DO
	return 0;
}

/* MAGICAL POWER */
unsigned int Player::getMagicalPowerBasis(){
	return magicalPowerBasis;
}
void Player::setMagicalPowerBasis(unsigned int magicalPowerBasis){
	this->magicalPowerBasis = magicalPowerBasis;
}
unsigned int Player::getTotalMagicalPower(){
	// TO DO
	return 0;
}

/* SKILLS */
std::vector<Skill>* Player::getSkills(){
	return skills;
}
Skill* Player::getSkillOfSlot(SkillSlot slot){
	for (unsigned int i = 0; i < skills->size(); i++)
		if ((*skills)[i].getSlot() == slot)
			return &(*skills)[i];
	return NULL;
}
bool Player::addNewSkill(Skill skill){
	if (this->skills->size() < MAXIMUM_SKILLS)
	{
		this->skills->push_back(skill);
		return true;
	}
	else
		return false;
}

/* EQUIPMENTS */
std::vector<Equipment>* Player::getEquipments(){
	return equipments;
}
Equipment* Player::getEquipmentOfClass(EquipmentClass equipmentClass){

	for (unsigned int i = 0; i < equipments->size(); i++)
		if ((*equipments)[i].getEquipmentClass() == equipmentClass)
			return &(*equipments)[i];

	return NULL;
}
Equipment* Player::addNewEquipment(Equipment equipment){
	for (unsigned int i = 0; i < equipments->size(); i++)
	{
		if ((*equipments)[i].getEquipmentClass() == equipment.getEquipmentClass())
		{
			equipments->erase(equipments->begin() + i);
			equipments->push_back(equipment);
			return &(*equipments)[i];
		}
	}

	equipments->push_back(equipment);
	return NULL;
}