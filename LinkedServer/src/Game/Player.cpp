#include "Player.h"
#include "Monster.h"
#include "Map.h"
#include <iostream>

Player::Player(glm::vec3 initialPosition, std::vector<Monster*>* monsters, Map* map)
{
	setName(PLAYER_DEFAULT_NAME);
	setHp(PLAYER_DEFAULT_HP);
	setMaximumHpBasis(PLAYER_DEFAULT_MAX_HP_BASIS);
	setAttackBasis(PLAYER_DEFAULT_ATTACK_BASIS);
	setDefenseBasis(PLAYER_DEFAULT_DEFENSE_BASIS);
	setMagicalPowerBasis(PLAYER_DEFAULT_MAGICAL_POWER_BASIS);
	setSpeedBasis(PLAYER_DEFAULT_SPEED_BASIS);
	setAttackSpeedBasis(PLAYER_DEFAULT_ATTACK_SPEED_BASIS);
	setPosition(initialPosition);
	skills = std::vector<Skill*>();
	equipments = std::vector<Equipment*>();
	this->clientId = 0;
	this->attributesChanged = false;
	this->link = nullptr;
}

Player::~Player()
{
}

Player* Player::getLink()
{
	return this->link;
}

void Player::setLink(Player* player)
{
	std::cout << "player " << this->getClientId() << " linked with player " << player->getClientId() << std::endl;
	this->link = link;
}

unsigned short Player::getClientId()
{
	return this->clientId;
}

void Player::setClientId(unsigned short clientId)
{
	this->clientId = clientId;
}

bool Player::didAttributesChanged()
{
	return this->attributesChanged;
}

void Player::resetAttributesChanged()
{
	this->attributesChanged = false;
}

bool Player::isFogOfWar(glm::vec3 position)
{
	glm::vec3 diffVector = position - this->getPosition();
	float vecLength = glm::length(diffVector);

	if (vecLength > PLAYER_FOG_OF_WAR_RADIUS)
		return true;
	else
		return false;
}

std::string Player::getName(){
	return name;
}
void Player::setName(std::string name){
	this->name = name;
}

bool Player::isAlive(){
	return getHp() != 0;
}

/* HP */
unsigned int Player::getHp()
{
	return hp;
}

void Player::setHp(unsigned int hp)
{
	this->hp = hp;
	this->attributesChanged = true;
}

void Player::doDamage(unsigned int damage)
{
	if (damage >= hp)
		hp = 0;
	else
	{
		//std::cout << "player hp: " << hp - damage << std::endl;
		hp = hp - damage;
	}

	this->attributesChanged = true;
}

void Player::healHp(unsigned int healingAmount){
	unsigned int maximumHp = getTotalMaximumHp();
	if ((healingAmount + hp) > maximumHp)
		hp = maximumHp;
	else
		hp = healingAmount + hp;

	this->attributesChanged = true;
}
void Player::restoreHpToMaximum(){
	hp = getTotalMaximumHp();
	this->attributesChanged = true;
}
unsigned int Player::getMaximumHpBasis(){
	return maximumHpBasis;
}
void Player::setMaximumHpBasis(unsigned int maximumHpBasis){
	this->maximumHpBasis = maximumHpBasis;
	this->attributesChanged = true;
}
unsigned int Player::getTotalMaximumHp(){
	// TO DO
	return getMaximumHpBasis();
}

/* ATTACK */
unsigned int Player::getAttackBasis(){
	return attackBasis;
}
void Player::setAttackBasis(unsigned int attackBasis){
	this->attackBasis = attackBasis;
	this->attributesChanged = true;
}
unsigned int Player::getTotalAttack(){
	// TO DO
	return getAttackBasis();
}

/* DEFENSE */
unsigned int Player::getDefenseBasis(){
	return defenseBasis;
}
void Player::setDefenseBasis(unsigned int defenseBasis){
	this->defenseBasis = defenseBasis;
	this->attributesChanged = true;
}
unsigned int Player::getTotalDefense(){
	// TO DO
	return getDefenseBasis();
}

/* MAGICAL POWER */
unsigned int Player::getMagicalPowerBasis(){
	return magicalPowerBasis;
}
void Player::setMagicalPowerBasis(unsigned int magicalPowerBasis){
	this->magicalPowerBasis = magicalPowerBasis;
	this->attributesChanged = true;
}
unsigned int Player::getTotalMagicalPower(){
	// TO DO
	return getMagicalPowerBasis();
}

/* SPEED */
unsigned int Player::getSpeedBasis(){
	return speedBasis;
}
void Player::setSpeedBasis(unsigned int speedBasis){
	this->speedBasis = speedBasis;
	this->attributesChanged = true;
}
unsigned int Player::getTotalSpeed(){
	// TO DO
	return getSpeedBasis();
}

/* ATTACK SPEED */
unsigned int Player::getAttackSpeedBasis(){
	return attackSpeedBasis;
}
void Player::setAttackSpeedBasis(unsigned int attackSpeedBasis){
	this->attackSpeedBasis = attackSpeedBasis;
	this->attributesChanged = true;
}
unsigned int Player::getTotalAttackSpeed(){
	// TO DO
	return getAttackSpeedBasis();
}

/* SKILLS */
std::vector<Skill*> Player::getSkills(){
	return skills;
}
Skill* Player::getSkillOfSlot(SkillSlot slot){
	for (unsigned int i = 0; i < skills.size(); i++)
		if (skills[i]->getSlot() == slot)
			return skills[i];
	return NULL;
}
bool Player::addNewSkill(Skill* skill){
	if (this->skills.size() < PLAYER_MAXIMUM_SKILLS)
	{
		skill->setWorldObject(this);
		this->skills.push_back(skill);
		return true;
	}
	else
		return false;
}
bool Player::isPlayerUsingASkill()
{
	if (this->getSkillOfSlot(SLOT_1) != nullptr && this->getSkillOfSlot(SLOT_1)->isActive()) return true;
	if (this->getSkillOfSlot(SLOT_2) != nullptr && this->getSkillOfSlot(SLOT_2)->isActive()) return true;
	if (this->getSkillOfSlot(SLOT_3) != nullptr && this->getSkillOfSlot(SLOT_3)->isActive()) return true;
	if (this->getSkillOfSlot(SLOT_4) != nullptr && this->getSkillOfSlot(SLOT_4)->isActive()) return true;

	return false;
}
bool Player::isPlayerUsingSkillOfSlot(SkillSlot slot)
{
	if (this->getSkillOfSlot(slot) == NULL)
		return false;
	return this->getSkillOfSlot(slot)->isActive();
}

/* EQUIPMENTS */
std::vector<Equipment*> Player::getEquipments(){
	return equipments;
}
Equipment* Player::getEquipmentOfClass(EquipmentClass equipmentClass){

	for (unsigned int i = 0; i < equipments.size(); i++)
		if (equipments[i]->getEquipmentClass() == equipmentClass)
			return equipments[i];

	return NULL;
}
Equipment* Player::addNewEquipment(Equipment* equipment){
	for (unsigned int i = 0; i < equipments.size(); i++)
	{
		if (equipments[i]->getEquipmentClass() == equipment->getEquipmentClass())
		{
			equipments.erase(equipments.begin() + i);
			equipments.push_back(equipment);
			return equipments[i];
		}
	}

	equipments.push_back(equipment);
	return NULL;
}

/* UPDATE */
void Player::update()
{
	for (Skill* skill : this->skills)
		skill->update();
}

/* NETWORK MOVEMENT */
bool Player::mustUpdateDestinationToClients()
{
	glm::vec3 currentPosition = this->getPosition();

	if (oldPosition.x != currentPosition.x || oldPosition.y != currentPosition.y)
		return true;
	else
		return false;
}

glm::vec3 Player::getDestination()
{
	this->oldPosition = this->getPosition();
	return this->getPosition();
}