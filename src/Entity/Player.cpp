#include "Player.h"
#include "HPBar.h"
#include "LinkedTime.h"
#include "Input.h"
#include "Primitive.h"
#include "Display.h"
#include "RangeAttack.h"
#include "Map.h"
#include "PacketController.h"
#include "Projectile.h"
#include "Game.h"

#include <iostream>

Player::Player(Transform* transform, Mesh* mesh, Texture* texture, std::vector<Monster*>* monsters, Map* map) : Entity(transform, mesh, texture)
{
	setName(PLAYER_DEFAULT_NAME);
	setHp(PLAYER_DEFAULT_HP);
	skills = std::vector<Skill*>();
	equipments = std::vector<Equipment*>();
	this->hpBar = new HPBar(this);
	this->rangeAttack = new RangeAttack(this, &attacks, monsters, map);
	this->ai = new PlayerAI();
	this->isMovingTo = false;
	this->type = LOCAL;
	setMaximumHpBasis(PLAYER_DEFAULT_MAX_HP_BASIS);
	setTotalMaximumHp(PLAYER_DEFAULT_MAX_HP_BASIS);
	setAttackBasis(PLAYER_DEFAULT_ATTACK_BASIS);
	setTotalAttack(PLAYER_DEFAULT_ATTACK_BASIS);
	setDefenseBasis(PLAYER_DEFAULT_DEFENSE_BASIS);
	setTotalDefense(PLAYER_DEFAULT_DEFENSE_BASIS);
	setMagicalPowerBasis(PLAYER_DEFAULT_MAGICAL_POWER_BASIS);
	setTotalMagicalPower(PLAYER_DEFAULT_MAGICAL_POWER_BASIS);
	setSpeedBasis(PLAYER_DEFAULT_SPEED_BASIS);
	setTotalSpeed(PLAYER_DEFAULT_SPEED_BASIS);
	setAttackSpeedBasis(PLAYER_DEFAULT_ATTACK_SPEED_BASIS);
	setTotalAttackSpeed(PLAYER_DEFAULT_ATTACK_SPEED_BASIS);

	this->currentDirection = PLAYER_FIRST_DIRECTION;
	this->lastIndexTexture = PLAYER_FIRST_INDEX_TEXTURE;
}

Player::~Player()
{
	delete hpBar;
	delete rangeAttack;
	delete ai;
}

/* *********************************** */
/* ********** PUBLIC METHODS ********* */
/* *********************************** */

/* BASIC ATTRIBUTES */

short Player::getClientId()
{
	return this->clientId;
}

void Player::setClientId(short clientId)
{
	this->clientId = clientId;
}

PlayerType Player::getType()
{
	return this->type;
}

void Player::setType(PlayerType type)
{
	this->type = type;
}

std::string Player::getName(){
	return name;
}

void Player::setName(std::string name){
	this->name = name;
}

unsigned int Player::getHp()
{
	return hp;
}

void Player::setHp(unsigned int hp)
{
	this->hp = hp;
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
	if (Game::multiplayer)
		return maximumHp;
	else
		return getMaximumHpBasis();
}

void Player::setTotalMaximumHp(unsigned int maxHp)
{
	this->maximumHp = maxHp;
}

unsigned int Player::getAttackBasis(){
	return attackBasis;
}

void Player::setAttackBasis(unsigned int attackBasis){
	this->attackBasis = attackBasis;
}

unsigned int Player::getTotalAttack(){
	if (Game::multiplayer)
		return attack;
	else
		return getAttackBasis();
}

void Player::setTotalAttack(unsigned int attack)
{
	this->attack = attack;
}

unsigned int Player::getDefenseBasis(){
	return defenseBasis;
}

void Player::setDefenseBasis(unsigned int defenseBasis){
	this->defenseBasis = defenseBasis;
}

unsigned int Player::getTotalDefense(){
	if (Game::multiplayer)
		return defense;
	else
		return getDefenseBasis();
}

void Player::setTotalDefense(unsigned int defense)
{
	this->defense = defense;
}

unsigned int Player::getMagicalPowerBasis(){
	return magicalPowerBasis;
}

void Player::setMagicalPowerBasis(unsigned int magicalPowerBasis){
	this->magicalPowerBasis = magicalPowerBasis;
}

unsigned int Player::getTotalMagicalPower(){
	if (Game::multiplayer)
		return magicalPower;
	else
		return getMagicalPowerBasis();
}

void Player::setTotalMagicalPower(unsigned int magicalPower)
{
	this->magicalPower = magicalPower;
}

unsigned int Player::getSpeedBasis(){
	return speedBasis;
}

void Player::setSpeedBasis(unsigned int speedBasis){
	this->speedBasis = speedBasis;
}

unsigned int Player::getTotalSpeed(){
	if (Game::multiplayer)
		return speed;
	else
		return getSpeedBasis();
}

void Player::setTotalSpeed(unsigned int speed)
{
	this->speed = speed;
}

unsigned int Player::getAttackSpeedBasis(){
	return attackSpeedBasis;
}

void Player::setAttackSpeedBasis(unsigned int attackSpeedBasis){
	this->attackSpeedBasis = attackSpeedBasis;
}

unsigned int Player::getTotalAttackSpeed(){
	if (Game::multiplayer)
		return attackSpeed;
	else
		return getAttackSpeedBasis();
}

void Player::setTotalAttackSpeed(unsigned int attackSpeed)
{
	this->attackSpeed = attackSpeed;
}

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
		skill->setEntity(this);
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

HPBar* Player::getHPBar()
{
	return hpBar;
}

RangeAttack* Player::getRangeAttack()
{
	return this->rangeAttack;
}

/* STATUS */

bool Player::isAlive(){
	return getHp() != 0;
}

bool Player::isMoving()
{
	return this->moving;
}

bool Player::isAttacking()
{
	if (this->attacking)
	{
		double now = Time::getTime();
		if ((now - lastAttackTime) > ((1.0f / getTotalAttackSpeed()) * 10.0f))
		{
			this->attacking = false;
			return false;
		}
		else
			return true;
	}
	return false;
}

void Player::doAttack()
{
	this->attacking = true;
	lastAttackTime = Time::getTime();
}

void Player::receiveDamage()
{
	this->receivingDamage = true;
	lastReceivedDamageTime = Time::getTime();
}

bool Player::isReceivingDamage()
{
	if (this->receivingDamage)
	{
		double now = Time::getTime();
		if ((now - lastReceivedDamageTime) > PLAYER_RECEIVE_DAMAGE_DELAY)
		{
			this->receivingDamage = false;
			return false;
		}
		else
			return true;
	}
	return false;
}

/* COMBAT */

void Player::doDamage(unsigned int damage)
{
	if (damage > hp)
		hp = 0;
	else
		hp = hp - damage;

	this->receiveDamage();
}

/* MOVEMENT */

void Player::startMovementTo(glm::vec3 destination)
{
	this->destination = destination;
	this->isMovingTo = true;
}

/* AUXILIAR */

bool Player::isFogOfWar(glm::vec3 position)
{
	glm::vec3 diffVector = position - this->getTransform()->getPosition();
	float vecLength = glm::length(diffVector);

	if (vecLength > PLAYER_FOG_OF_WAR_RADIUS)
		return true;
	else
		return false;
}

/* METHODS RELATED TO INPUT, UPDATE AND RENDERING */

void Player::update(Map* map)
{
	if (Game::multiplayer)
	{
		this->updateMovement(map);
	}

	this->hpBar->update();
	this->rangeAttack->update();
	this->refreshTexture();

	for (Skill* skill : this->skills)
		skill->update();
}

void Player::input(Map* map)
{
	this->hpBar->input();
	if (this->rangeAttack != NULL) this->rangeAttack->input();

	glm::vec3 currentPosition = this->getTransform()->getPosition();

	if (!Game::multiplayer)
	{
		if (Input::keyStates['x'] && this->getHp() != this->getTotalMaximumHp())
			this->setHp(this->getHp() + 1);
	}

	this->moving = false;

	if (this->isAlive())
	{
		if (Input::keyStates['w'] && !Input::keyStates['a'] && !Input::keyStates['s'] && !Input::keyStates['d'])
		{
			glm::vec3 deltaVector = this->getDeltaVectorToDirection(TOP);
			if (checkIfPlayerIsStillOnTheSameMapPosition(currentPosition, currentPosition + deltaVector) || !map->coordinateHasCollision(currentPosition + deltaVector))
				this->getTransform()->incTranslate(deltaVector.x, deltaVector.y, deltaVector.z);
			this->currentDirection = TOP;
			this->moving = true;
		}
		else if (Input::keyStates['w'] && !Input::keyStates['a'] && !Input::keyStates['s'] && Input::keyStates['d'])
		{
			glm::vec3 deltaVector = this->getDeltaVectorToDirection(TOP_RIGHT);
			if (checkIfPlayerIsStillOnTheSameMapPosition(currentPosition, currentPosition + deltaVector) || !map->coordinateHasCollision(currentPosition + deltaVector))
				this->getTransform()->incTranslate(deltaVector.x, deltaVector.y, deltaVector.z);
			this->currentDirection = TOP_RIGHT;
			this->moving = true;
		}
		else if (!Input::keyStates['w'] && !Input::keyStates['a'] && !Input::keyStates['s'] && Input::keyStates['d'])
		{
			glm::vec3 deltaVector = this->getDeltaVectorToDirection(RIGHT);
			if (checkIfPlayerIsStillOnTheSameMapPosition(currentPosition, currentPosition + deltaVector) || !map->coordinateHasCollision(currentPosition + deltaVector))
				this->getTransform()->incTranslate(deltaVector.x, deltaVector.y, deltaVector.z);
			this->currentDirection = RIGHT;
			this->moving = true;
		}
		else if (!Input::keyStates['w'] && !Input::keyStates['a'] && Input::keyStates['s'] && Input::keyStates['d'])
		{
			glm::vec3 deltaVector = this->getDeltaVectorToDirection(BOTTOM_RIGHT);
			if (checkIfPlayerIsStillOnTheSameMapPosition(currentPosition, currentPosition + deltaVector) || !map->coordinateHasCollision(currentPosition + deltaVector))
				this->getTransform()->incTranslate(deltaVector.x, deltaVector.y, deltaVector.z);
			this->currentDirection = BOTTOM_RIGHT;
			this->moving = true;
		}
		else if (!Input::keyStates['w'] && !Input::keyStates['a'] && Input::keyStates['s'] && !Input::keyStates['d'])
		{
			glm::vec3 deltaVector = this->getDeltaVectorToDirection(BOTTOM);
			if (checkIfPlayerIsStillOnTheSameMapPosition(currentPosition, currentPosition + deltaVector) || !map->coordinateHasCollision(currentPosition + deltaVector))
				this->getTransform()->incTranslate(deltaVector.x, deltaVector.y, deltaVector.z);
			this->currentDirection = BOTTOM;
			this->moving = true;
		}
		else if (!Input::keyStates['w'] && Input::keyStates['a'] && Input::keyStates['s'] && !Input::keyStates['d'])
		{
			glm::vec3 deltaVector = this->getDeltaVectorToDirection(BOTTOM_LEFT);
			if (checkIfPlayerIsStillOnTheSameMapPosition(currentPosition, currentPosition + deltaVector) || !map->coordinateHasCollision(currentPosition + deltaVector))
				this->getTransform()->incTranslate(deltaVector.x, deltaVector.y, deltaVector.z);
			this->currentDirection = BOTTOM_LEFT;
			this->moving = true;
		}
		else if (!Input::keyStates['w'] && Input::keyStates['a'] && !Input::keyStates['s'] && !Input::keyStates['d'])
		{
			glm::vec3 deltaVector = this->getDeltaVectorToDirection(LEFT);
			if (checkIfPlayerIsStillOnTheSameMapPosition(currentPosition, currentPosition + deltaVector) || !map->coordinateHasCollision(currentPosition + deltaVector))
				this->getTransform()->incTranslate(deltaVector.x, deltaVector.y, deltaVector.z);
			this->currentDirection = LEFT;
			this->moving = true;
		}
		else if (Input::keyStates['w'] && Input::keyStates['a'] && !Input::keyStates['s'] && !Input::keyStates['d'])
		{
			glm::vec3 deltaVector = this->getDeltaVectorToDirection(TOP_LEFT);
			if (checkIfPlayerIsStillOnTheSameMapPosition(currentPosition, currentPosition + deltaVector) || !map->coordinateHasCollision(currentPosition + deltaVector))
				this->getTransform()->incTranslate(deltaVector.x, deltaVector.y, deltaVector.z);
			this->currentDirection = TOP_LEFT;
			this->moving = true;
		}

		if (Input::keyStates['z'] && this->getSkillOfSlot(SLOT_1) != NULL && !this->isPlayerUsingASkill())
			this->getSkillOfSlot(SLOT_1)->prepareExecution(this->currentDirection);
		else if (Input::keyStates['x'] && this->getSkillOfSlot(SLOT_2) != NULL && !this->isPlayerUsingASkill())
			this->getSkillOfSlot(SLOT_2)->prepareExecution(this->currentDirection);
		else if (Input::keyStates['c'] && this->getSkillOfSlot(SLOT_3) != NULL && !this->isPlayerUsingASkill())
			this->getSkillOfSlot(SLOT_3)->prepareExecution(this->currentDirection);
		else if (Input::keyStates['v'] && this->getSkillOfSlot(SLOT_4) != NULL && !this->isPlayerUsingASkill())
			this->getSkillOfSlot(SLOT_4)->prepareExecution(this->currentDirection);

		if (Input::leftMouseButton)
		{
			if (this->isPlayerUsingSkillOfSlot(SLOT_1))
				this->getSkillOfSlot(SLOT_1)->cancelIfPossible();
			else if (this->isPlayerUsingSkillOfSlot(SLOT_2))
				this->getSkillOfSlot(SLOT_2)->cancelIfPossible();
			else if (this->isPlayerUsingSkillOfSlot(SLOT_3))
				this->getSkillOfSlot(SLOT_3)->cancelIfPossible();
			else if (this->isPlayerUsingSkillOfSlot(SLOT_4))
				this->getSkillOfSlot(SLOT_4)->cancelIfPossible();

			Input::leftMouseButton = false;
		}
	}
}

void Player::render(Shader* primitiveShader, TextRenderer* textRenderer, Shader* projectileShader)
{
	Entity::render(primitiveShader);
	this->getHPBar()->quad->render(primitiveShader);

	// Projectile attacks
	for (Entity* e : this->attacks)
	{
		try{
			projectileShader->activateAlphaBlend();
			e->render(projectileShader);
			projectileShader->deactivateAlphaBlend();
		}
		catch (...){
			std::cerr << "Error rendering entity" << std::endl;
		}
	}

	for (Skill* skill : this->getSkills())
	{
		try{
			if (skill->isActive())
				skill->render(primitiveShader, textRenderer);
		}
		catch (...){
			std::cerr << "Error rendering entity" << std::endl;
		}
	}
}

/* *********************************** */
/* ********** PRIVATE METHODS ********* */
/* *********************************** */

/* TEXTURE-RELATED METHODS */

void Player::refreshTexture()
{
	double now = Time::getTime();
	bool shouldChangeTexture = (now - textureChangeTime) > PLAYER_TEXTURE_CHANGE_DELAY;

	bool isDead = !this->isAlive();
	bool isAttacking = this->isAttacking();
	bool isReceivingDamage = this->isReceivingDamage();
	bool isMoving = this->isMoving();

	if (shouldChangeTexture || this->currentDirection != this->lastDirection || isDead != lastIsDead || isAttacking != lastIsAttacking || isReceivingDamage != lastIsReceivingDamage || isMoving != lastIsMoving)
	{
		if (isDead)
			changeTextureBasedOnDirection(this->currentDirection, 28, 28);
		else
		{
			switch (this->currentDirection)
			{
			case TOP:
				if (!isAttacking && !isReceivingDamage && isMoving) changeTextureBasedOnDirection(this->currentDirection, 112, 115);
				else if (isReceivingDamage) changeTextureBasedOnDirection(this->currentDirection, 75, 77);
				else if (isAttacking) changeTextureBasedOnDirection(this->currentDirection, 51, 53);
				else if (!isMoving) changeTextureBasedOnDirection(this->currentDirection, 135, 137);
				break;
			case TOP_LEFT:
				if (!isAttacking && !isReceivingDamage && isMoving) changeTextureBasedOnDirection(this->currentDirection, 116, 119);
				else if (isReceivingDamage) changeTextureBasedOnDirection(this->currentDirection, 78, 80);
				else if (isAttacking) changeTextureBasedOnDirection(this->currentDirection, 54, 56);
				else if (!isMoving) changeTextureBasedOnDirection(this->currentDirection, 138, 140);
				break;
			case TOP_RIGHT:
				if (!isAttacking && !isReceivingDamage && isMoving) changeTextureBasedOnDirection(this->currentDirection, 108, 111);
				else if (isReceivingDamage) changeTextureBasedOnDirection(this->currentDirection, 72, 74);
				else if (isAttacking) changeTextureBasedOnDirection(this->currentDirection, 48, 50);
				else if (!isMoving) changeTextureBasedOnDirection(this->currentDirection, 132, 134);
				break;
			case RIGHT:
				if (!isAttacking && !isReceivingDamage && isMoving) changeTextureBasedOnDirection(this->currentDirection, 88, 91);
				else if (isReceivingDamage) changeTextureBasedOnDirection(this->currentDirection, 69, 71);
				else if (isAttacking) changeTextureBasedOnDirection(this->currentDirection, 45, 47);
				else if (!isMoving) changeTextureBasedOnDirection(this->currentDirection, 129, 131);
				break;
			case BOTTOM_RIGHT:
				if (!isAttacking && !isReceivingDamage && isMoving) changeTextureBasedOnDirection(this->currentDirection, 84, 87);
				else if (isReceivingDamage) changeTextureBasedOnDirection(this->currentDirection, 66, 68);
				else if (isAttacking) changeTextureBasedOnDirection(this->currentDirection, 42, 44);
				else if (!isMoving) changeTextureBasedOnDirection(this->currentDirection, 126, 128);
				break;
			case BOTTOM:
				if (!isAttacking && !isReceivingDamage && isMoving) changeTextureBasedOnDirection(this->currentDirection, 104, 107);
				else if (isReceivingDamage) changeTextureBasedOnDirection(this->currentDirection, 63, 65);
				else if (isAttacking) changeTextureBasedOnDirection(this->currentDirection, 39, 41);
				else if (!isMoving) changeTextureBasedOnDirection(this->currentDirection, 123, 125);
				break;
			case LEFT:
				if (!isAttacking && !isReceivingDamage && isMoving) changeTextureBasedOnDirection(this->currentDirection, 96, 99);
				else if (isReceivingDamage) changeTextureBasedOnDirection(this->currentDirection, 81, 83);
				else if (isAttacking) changeTextureBasedOnDirection(this->currentDirection, 57, 59);
				else if (!isMoving) changeTextureBasedOnDirection(this->currentDirection, 141, 143);
				break;
			case BOTTOM_LEFT:
				if (!isAttacking && !isReceivingDamage && isMoving) changeTextureBasedOnDirection(this->currentDirection, 100, 103);
				else if (isReceivingDamage) changeTextureBasedOnDirection(this->currentDirection, 60, 62);
				else if (isAttacking) changeTextureBasedOnDirection(this->currentDirection, 36, 38);
				else if (!isMoving) changeTextureBasedOnDirection(this->currentDirection, 120, 122);
				break;
			}
		}

		this->lastDirection = this->currentDirection;
		this->lastIsAttacking = this->isAttacking();
		this->lastIsReceivingDamage = this->isReceivingDamage();
		this->lastIsDead = !this->isAlive();
		this->lastIsMoving = this->isMoving();
		this->textureChangeTime = now;
	}
}

void Player::changeTextureBasedOnDirection(MovementDirection direction, unsigned int initialTextureIndex, unsigned int finalTextureIndex)
{
	if (direction != this->lastDirection || this->isAttacking() != lastIsAttacking || this->isReceivingDamage() != lastIsReceivingDamage || !this->isAlive() != lastIsDead || this->isMoving() != lastIsMoving)
	{
		this->getMesh()->getQuad()->setIndex(initialTextureIndex);
		this->lastIndexTexture = initialTextureIndex;
	}
	else
	{
		if (this->lastIndexTexture < finalTextureIndex)
		{
			this->getMesh()->getQuad()->setIndex(++this->lastIndexTexture);
		}
		else
		{
			this->getMesh()->getQuad()->setIndex(initialTextureIndex);
			this->lastIndexTexture = initialTextureIndex;
		}
	}
}

/* MOVEMENT METHODS */

void Player::updateMovement(Map* map)
{
	if (isMovingTo)
	{
		glm::vec3 pPos = this->getTransform()->getPosition();
		float frameTime = (float)Display::frameTime;
		float range = frameTime * this->getTotalSpeed();
		MovementDefinition newPos = this->ai->movePerfectlyTo(map, pPos, this->destination, range);

		if (newPos.doMove)
		{
			this->getTransform()->translate(newPos.movement.x, newPos.movement.y, newPos.movement.z);
			this->currentDirection = newPos.direction;
			this->moving = true;
		}
		else
		{
			this->moving = false;
			this->isMovingTo = false;
		}
	}
}

/* AUXILIAR METHODS */

bool Player::checkIfPlayerIsStillOnTheSameMapPosition(glm::vec3 currentPosition, glm::vec3 nextPosition)
{
	if (floor(currentPosition.x) == floor(nextPosition.x))
		if (floor(currentPosition.y) == floor(nextPosition.y))
			if (floor(currentPosition.z) == floor(nextPosition.z))
				return true;

	return false;
}

glm::vec3 Player::getDeltaVectorToDirection(MovementDirection direction)
{
	float frameTime = (float)Display::frameTime;
	float range = frameTime * this->getTotalSpeed();

	switch (direction)
	{
	case TOP:
		return glm::vec3(0, range, 0);
	case TOP_RIGHT:
		return glm::vec3(range / SQRT2, range / SQRT2, 0);
	case RIGHT:
		return glm::vec3(range, 0, 0);
	case BOTTOM_RIGHT:
		return glm::vec3(range / SQRT2, -range / SQRT2, 0);
	case BOTTOM:
		return glm::vec3(0, -range, 0);
	case BOTTOM_LEFT:
		return glm::vec3(-range / SQRT2, -range / SQRT2, 0);
	case LEFT:
		return glm::vec3(-range, 0, 0);
	case TOP_LEFT:
		return glm::vec3(-range / SQRT2, range / SQRT2, 0);
	default:
		return glm::vec3(0, 0, 0);
	}
}