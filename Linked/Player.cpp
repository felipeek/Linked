#include "Player.h"
#include "HPBar.h"

Player::Player(Transform* transform, Mesh* mesh, Texture* texture) : Entity(transform, mesh, texture)
{
	setName(PLAYER_DEFAULT_NAME);
	setHp(PLAYER_DEFAULT_HP);
	setMaximumHpBasis(PLAYER_DEFAULT_MAX_HP_BASIS);
	setLives(PLAYER_DEFAULT_LIVES);
	setMaximumLivesBasis(PLAYER_DEFAULT_MAX_LIVES_BASIS);
	setAttackBasis(PLAYER_DEFAULT_ATTACK_BASIS);
	setDefenseBasis(PLAYER_DEFAULT_DEFENSE_BASIS);
	setMagicalPowerBasis(PLAYER_DEFAULT_MAGICAL_POWER_BASIS);
	setSpeedBasis(PLAYER_DEFAULT_SPEED_BASIS);
	setAttackSpeedBasis(PLAYER_DEFAULT_ATTACK_SPEED_BASIS);
	skills = new std::vector<Skill>();
	equipments = new std::vector<Equipment>();
	this->hpBar = new HPBar(this);
}

Player::~Player()
{
	delete skills;
	delete equipments;
	delete hpBar;
}

void Player::update()
{
	hpBar->update();
}

void Player::input()
{
	hpBar->input();
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
}

void Player::doDamage(unsigned int damage)
{
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
	return getMaximumHpBasis();
}

/* LIVES */
unsigned int Player::getLives(){
	return lives;
}
void Player::setLives(unsigned int lives){
	this->lives = lives;
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
void Player::setMaximumLivesBasis(unsigned int maximumLivesBasis){
	this->maximumLivesBasis = maximumLivesBasis;
}
unsigned int Player::getTotalMaximumLives(){
	// TO DO
	return getMaximumLivesBasis();
}

/* ATTACK */
unsigned int Player::getAttackBasis(){
	return attackBasis;
}
void Player::setAttackBasis(unsigned int attackBasis){
	this->attackBasis = attackBasis;
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
}
unsigned int Player::getTotalAttackSpeed(){
	// TO DO
	return getAttackSpeedBasis();
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
	if (this->skills->size() < PLAYER_MAXIMUM_SKILLS)
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

/* HP BAR */
HPBar* Player::getHPBar()
{
	return hpBar;
}