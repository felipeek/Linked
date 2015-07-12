#include "Player.h"


Player::Player(unsigned int hpMaximum, unsigned int livesMaximum, std::vector<Skill> &skills, std::vector<Equipment> &equipments, int basisAttack, int basisDefense, float basisRange)
{
	this->hpMaximum = hpMaximum;
	this->livesMaximum = livesMaximum;
	this->basisAttack = basisAttack;
	this->basisDefense = basisDefense;
	this->basisRange = basisRange;

	this->hp = hpMaximum;
	this->lives = livesMaximum;
	this->status = NORMAL;
	this->skills = skills;
	this->equipments = equipments;
}

Player::~Player()
{
}
unsigned int Player::getHp(){
	return hp;
}
unsigned int Player::getLives(){
	return lives;
}
PlayerStatus Player::getStatus(){
	return status;
}
std::vector<Skill> Player::getSkills(){
	return skills;
}
Equipment* Player::getEquipment(EquipmentClass equipmentClass){
	for (Equipment equip : equipments)
		if (equip.getEquipmentClass() == equipmentClass)
			return &equip;

	return NULL;
}
void Player::changeStatus(PlayerStatus status){
	this->status = status;
}
void Player::addSkill(Skill skill){

}
// Adds equipment to the correct slot
// If another equipment was already occuping that slot, it will be popped from equipments vector and returned.
Equipment Player::addEquipment(Equipment equipment)
{
	return Equipment();
}