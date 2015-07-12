#include "Player.h"


Player::Player()
{

}

Player::~Player()
{
}

/* HP */

//std::string Player::getName(){
//	return name;
//}
//void Player::setName(std::string name){
//	this->name = name;
//}
//unsigned int Player::getHp(){
//	return hp;
//}
//void Player::receiveDamage(unsigned int damage){
//	if (damage > hp)
//		hp = 0;
//	else
//		hp = hp - damage;
//}
//void Player::healHp(unsigned int healingAmount){
//	unsigned int maximumHp = getTotalMaximumHp();
//	if ((healingAmount + hp) > maximumHp)
//		hp = maximumHp;
//	else
//		hp = healingAmount + hp;
//}
//void Player::restoreHpToMaximum(){
//	hp = getTotalMaximumHp();
//}
//unsigned int Player::getMaximumHpBasis(){
//	return maximumHpBasis;
//}
//void Player::setMaximumHpBasis(unsigned int maximumHpBasis){
//	this->maximumHpBasis = maximumHpBasis;
//}
//unsigned int Player::getTotalMaximumHp(){
//	// TO DO
//}
//
///* LIVES */
//unsigned int Player::getLives(){
//	return lives;
//}
//void Player::removeLive(){
//	if (lives > 0)
//		lives--;
//}
//void Player::restoreAllLives(){
//	lives = getTotalMaximumLives();
//}
//void Player::restoreLive(){
//	if (lives < getTotalMaximumLives())
//		lives++;
//}
//unsigned int Player::getMaximumLivesBasis(){
//	return maximumLivesBasis;
//}
//unsigned int Player::getTotalMaximumLives(){
//	// TO DO
//}
//
///* ATTACK */
//unsigned int Player::getAttackBasis(){
//	return attackBasis;
//}
//void Player::setAttackBasis(unsigned int attackBasis){
//	this->attackBasis = attackBasis;
//}
//unsigned int getTotalAttack(){
//	// TO DO
//}
//
///* DEFENSE */
//unsigned int Player::getDefenseBasis(){
//	return defenseBasis;
//}
//void Player::setDefenseBasis(unsigned int defenseBasis){
//	this->defenseBasis = defenseBasis;
//}
//unsigned int Player::getTotalDefense(){
//	// TO DO
//}
//
///* MAGICAL POWER */
//unsigned int Player::getMagicalPowerBasis(){
//	return magicalPowerBasis;
//}
//void Player::setMagicalPowerBasis(unsigned int magicalPowerBasis){
//	this->magicalPowerBasis = magicalPowerBasis;
//}
//unsigned int Player::getTotalMagicalPower(){
//	// TO DO
//}
//
///* SKILLS */
//std::vector<Skill> Player::getSkills(){
//	return skills;
//}
//Skill Player::getSkillOfSlot(SkillSlot slot){
//	for (Skill skill : skills)
//		if (skill.getSlot() == slot)
//			return skill;
//}
//bool Player::addNewSkill(Skill skill){
//	if (this->skills.size() < MAXIMUM_SKILLS)
//	{
//		this->skills.push_back(skill);
//		return true;
//	}
//	else
//		return false;
//}
//
///* EQUIPMENTS */
//std::vector<Equipment> Player::getEquipments(){
//	return equipments;
//}
//Equipment* Player::getEquipmentOfClass(EquipmentClass equipmentClass){
//	for (Equipment equipment : equipments)
//		if (equipment.getEquipmentClass() == equipmentClass)
//			return &equipment;
//
//	return NULL;
//}
//Equipment* Player::addNewEquipment(Equipment equipment){
//	for (int i = 0; i < equipments.size(); i++)
//	{
//		if (equipments[i].getEquipmentClass() == equipment.getEquipmentClass())
//		{
//			Equipment equipmentToRemove = equipments[i];
//			equipments.erase(equipments.begin() + i);
//			equipments.push_back(equipment);
//			return &equipmentToRemove;
//		}
//	}
//
//	equipments.push_back(equipment);
//	return NULL;
//}