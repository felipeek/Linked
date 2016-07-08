#include "Player.h"
#include "HPBar.h"
#include "LinkedTime.h"
#include "Input.h"
#include "Primitive.h"
#include "RangeAttack.h"
#include "Map.h"
#include "Game.h"
#include "Skill.h"
#include "PlayerAI.h"
#include "Equipment.h"
#include "Projectile.h"
#include "Common.h"
#include "Audio.h"

Player::Player(Transform* transform, Mesh* mesh, Texture* texture) : Entity(transform, mesh, texture)
{
	setName(PLAYER_DEFAULT_NAME);
	setHp(PLAYER_DEFAULT_HP);
	skills = std::vector<Skill*>();
	equipments = std::vector<Equipment*>();
	this->link = NULL;
	this->hpBar = new HPBar(this);
	this->rangeAttack = new RangeAttack(this, &attacks);
	this->ai = new PlayerAI(*this);
	this->isMovingTo = false;
	this->type = LOCAL;
	this->hpBar_shouldRender = true;
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

	this->receiveDamageAudio = new Audio("./res/Audio/player_hit.wav", AudioType::SOUND);
	this->attackSound = new Audio("./res/Audio/attack.wav", AudioType::SOUND);
}

Player::~Player()
{
	delete hpBar;
	delete ai;

	for (unsigned int i = 0, skillsSize = skills.size(); i < skillsSize; i++)
	{
		delete skills[0];
		skills.erase(skills.begin());
	}
	for (unsigned int i = 0, equipsSize = equipments.size(); i < equipsSize; i++)
	{
		delete equipments[0];
		equipments.erase(equipments.begin());
	}
	
	for (unsigned int i = 0, attacksSize = attacks.size(); i < attacksSize; i++)
	{
		delete attacks[0];
		attacks.erase(attacks.begin());
	}
	delete rangeAttack;

	delete this->receiveDamageAudio;
	delete this->attackSound;
}

/* *********************************** */
/* ********** PUBLIC METHODS ********* */
/* *********************************** */

/* BASIC ATTRIBUTES */

Player* Player::getLink()
{
	return this->link;
}

void Player::setLink(Player* link)
{
	this->link = link;
}

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
		return totalMaximumHp;
	else
		return getMaximumHpBasis();
}

void Player::setTotalMaximumHp(unsigned int maxHp)
{
	this->totalMaximumHp = maxHp;
}

unsigned int Player::getAttackBasis(){
	return attackBasis;
}

void Player::setAttackBasis(unsigned int attackBasis){
	this->attackBasis = attackBasis;
}

unsigned int Player::getTotalAttack(){
	if (Game::multiplayer)
		return totalAttack;
	else
		return getAttackBasis();
}

void Player::setTotalAttack(unsigned int attack)
{
	this->totalAttack = attack;
}

unsigned int Player::getDefenseBasis(){
	return defenseBasis;
}

void Player::setDefenseBasis(unsigned int defenseBasis){
	this->defenseBasis = defenseBasis;
}

unsigned int Player::getTotalDefense(){
	if (Game::multiplayer)
		return totalDefense;
	else
		return getDefenseBasis();
}

void Player::setTotalDefense(unsigned int defense)
{
	this->totalDefense = defense;
}

unsigned int Player::getMagicalPowerBasis(){
	return magicalPowerBasis;
}

void Player::setMagicalPowerBasis(unsigned int magicalPowerBasis){
	this->magicalPowerBasis = magicalPowerBasis;
}

unsigned int Player::getTotalMagicalPower(){
	if (Game::multiplayer)
		return totalMagicalPower;
	else
		return getMagicalPowerBasis();
}

void Player::setTotalMagicalPower(unsigned int magicalPower)
{
	this->totalMagicalPower = magicalPower;
}

unsigned int Player::getSpeedBasis(){
	return speedBasis;
}

void Player::setSpeedBasis(unsigned int speedBasis){
	this->speedBasis = speedBasis;
}

unsigned int Player::getTotalSpeed(){
	if (Game::multiplayer)
		return totalSpeed;
	else
		return getSpeedBasis();
}

void Player::setTotalSpeed(unsigned int speed)
{
	this->totalSpeed = speed;
}

unsigned int Player::getAttackSpeedBasis(){
	return attackSpeedBasis;
}

void Player::setAttackSpeedBasis(unsigned int attackSpeedBasis){
	this->attackSpeedBasis = attackSpeedBasis;
}

unsigned int Player::getTotalAttackSpeed(){
	if (Game::multiplayer)
		return totalAttackSpeed;
	else
		return getAttackSpeedBasis();
}

void Player::setTotalAttackSpeed(unsigned int attackSpeed)
{
	this->totalAttackSpeed = attackSpeed;
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
	return this->hp != 0;
}

bool Player::isMoving()
{
	return this->moving;
}

bool Player::isAttacking()
{
	return this->attacking;
}

void Player::attack()
{
	this->attackSound->play();
	this->attacking = true;
	lastAttackTime = LinkedTime::getTime();
}

void Player::receiveDamage()
{
	this->receiveDamageAudio->play();
	this->receivingDamage = true;
	lastReceivedDamageTime = LinkedTime::getTime();
}

bool Player::isReceivingDamage()
{
	return this->receivingDamage;
}

/* COMBAT */

void Player::doDamage(unsigned int damage)
{
	if (damage >= hp)
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

bool Player::isOutsideExternalRadiusArea(glm::vec3 position)
{
	glm::vec3 diffVector = position - this->getTransform()->getPosition();
	float vecLength = glm::length(diffVector);

	if (vecLength > PLAYER_OUTSIDE_RADIUS_AREA)
		return true;
	else
		return false;
}

/* METHODS RELATED TO INPUT, UPDATE AND RENDERING */

void Player::update(Map* map, Player* localPlayer, std::vector<Player*>* players, std::vector<Monster*>* monsters)
{
	double now = LinkedTime::getTime();

	// UPDATE 'ATTACKING' STATUS
	if (this->attacking)
		if ((now - lastAttackTime) > ((1.0f / getTotalAttackSpeed()) * ASPD_FACTOR))
			this->attacking = false;

	// UPDATE 'RECEIVING DAMAGE' STATUS
	if (this->receivingDamage)
		if ((now - lastReceivedDamageTime) > RECEIVE_DAMAGE_DELAY)
			this->receivingDamage = false;

	if (Game::multiplayer)
		this->moveOnline(map);

	this->hpBar->update();
	this->rangeAttack->update(map, monsters);
	this->refreshTexture();

	for (Skill* skill : this->skills)
		skill->update(monsters, players, localPlayer);
}

void Player::input(Map* map)
{
	this->hpBar->input();
	if (this->rangeAttack != NULL) this->rangeAttack->input();

	glm::vec3 currentPosition = this->getTransform()->getPosition();

	if (!Game::multiplayer)
	{
		if (Input::keyStates['X'] && this->getHp() != this->getTotalMaximumHp())
			this->setHp(this->getHp() + 1);
	}

	this->moving = false;

	if (this->isAlive())
	{
		if (Input::keyStates['W'] && !Input::keyStates['A'] && !Input::keyStates['S'] && !Input::keyStates['D'])
		{
			glm::vec3 nextPosition = this->getPlayerNextPosition(TOP);
			if (checkIfPlayerIsStillOnTheSameMapPosition(currentPosition, nextPosition) || !map->coordinateHasCollision(nextPosition))
				this->getTransform()->translate(nextPosition.x, nextPosition.y, nextPosition.z);
			this->currentDirection = TOP;
			this->moving = true;
		}
		else if (Input::keyStates['W'] && !Input::keyStates['A'] && !Input::keyStates['S'] && Input::keyStates['D'])
		{
			glm::vec3 nextPosition = this->getPlayerNextPosition(TOP_RIGHT);
			if (checkIfPlayerIsStillOnTheSameMapPosition(currentPosition, nextPosition) || !map->coordinateHasCollision(nextPosition))
				this->getTransform()->translate(nextPosition.x, nextPosition.y, nextPosition.z);
			this->currentDirection = TOP_RIGHT;
			this->moving = true;
		}
		else if (!Input::keyStates['W'] && !Input::keyStates['A'] && !Input::keyStates['S'] && Input::keyStates['D'])
		{
			glm::vec3 nextPosition = this->getPlayerNextPosition(RIGHT);
			if (checkIfPlayerIsStillOnTheSameMapPosition(currentPosition, nextPosition) || !map->coordinateHasCollision(nextPosition))
				this->getTransform()->translate(nextPosition.x, nextPosition.y, nextPosition.z);
			this->currentDirection = RIGHT;
			this->moving = true;
		}
		else if (!Input::keyStates['W'] && !Input::keyStates['A'] && Input::keyStates['S'] && Input::keyStates['D'])
		{
			glm::vec3 nextPosition = this->getPlayerNextPosition(BOTTOM_RIGHT);
			if (checkIfPlayerIsStillOnTheSameMapPosition(currentPosition, nextPosition) || !map->coordinateHasCollision(nextPosition))
				this->getTransform()->translate(nextPosition.x, nextPosition.y, nextPosition.z);
			this->currentDirection = BOTTOM_RIGHT;
			this->moving = true;
		}
		else if (!Input::keyStates['W'] && !Input::keyStates['A'] && Input::keyStates['S'] && !Input::keyStates['D'])
		{
			glm::vec3 nextPosition = this->getPlayerNextPosition(BOTTOM);
			if (checkIfPlayerIsStillOnTheSameMapPosition(currentPosition, nextPosition) || !map->coordinateHasCollision(nextPosition))
				this->getTransform()->translate(nextPosition.x, nextPosition.y, nextPosition.z);
			this->currentDirection = BOTTOM;
			this->moving = true;
		}
		else if (!Input::keyStates['W'] && Input::keyStates['A'] && Input::keyStates['S'] && !Input::keyStates['D'])
		{
			glm::vec3 nextPosition = this->getPlayerNextPosition(BOTTOM_LEFT);
			if (checkIfPlayerIsStillOnTheSameMapPosition(currentPosition, nextPosition) || !map->coordinateHasCollision(nextPosition))
				this->getTransform()->translate(nextPosition.x, nextPosition.y, nextPosition.z);
			this->currentDirection = BOTTOM_LEFT;
			this->moving = true;
		}
		else if (!Input::keyStates['W'] && Input::keyStates['A'] && !Input::keyStates['S'] && !Input::keyStates['D'])
		{
			glm::vec3 nextPosition = this->getPlayerNextPosition(LEFT);
			if (checkIfPlayerIsStillOnTheSameMapPosition(currentPosition, nextPosition) || !map->coordinateHasCollision(nextPosition))
				this->getTransform()->translate(nextPosition.x, nextPosition.y, nextPosition.z);
			this->currentDirection = LEFT;
			this->moving = true;
		}
		else if (Input::keyStates['W'] && Input::keyStates['A'] && !Input::keyStates['S'] && !Input::keyStates['D'])
		{
			glm::vec3 nextPosition = this->getPlayerNextPosition(TOP_LEFT);
			if (checkIfPlayerIsStillOnTheSameMapPosition(currentPosition, nextPosition) || !map->coordinateHasCollision(nextPosition))
				this->getTransform()->translate(nextPosition.x, nextPosition.y, nextPosition.z);
			this->currentDirection = TOP_LEFT;
			this->moving = true;
		}

		if (Input::keyStates['1'] && this->getSkillOfSlot(SLOT_1) != NULL && !this->isPlayerUsingASkill())
			this->getSkillOfSlot(SLOT_1)->prepareExecution(this->currentDirection);
		else if (Input::keyStates['2'] && this->getSkillOfSlot(SLOT_2) != NULL && !this->isPlayerUsingASkill())
			this->getSkillOfSlot(SLOT_2)->prepareExecution(this->currentDirection);
		else if (Input::keyStates['3'] && this->getSkillOfSlot(SLOT_3) != NULL && !this->isPlayerUsingASkill())
			this->getSkillOfSlot(SLOT_3)->prepareExecution(this->currentDirection);
		else if (Input::keyStates['4'] && this->getSkillOfSlot(SLOT_4) != NULL && !this->isPlayerUsingASkill())
			this->getSkillOfSlot(SLOT_4)->prepareExecution(this->currentDirection);

		if (Input::rightMouseButton)
		{
			if (this->isPlayerUsingSkillOfSlot(SLOT_1))
				this->getSkillOfSlot(SLOT_1)->cancelIfPossible();
			else if (this->isPlayerUsingSkillOfSlot(SLOT_2))
				this->getSkillOfSlot(SLOT_2)->cancelIfPossible();
			else if (this->isPlayerUsingSkillOfSlot(SLOT_3))
				this->getSkillOfSlot(SLOT_3)->cancelIfPossible();
			else if (this->isPlayerUsingSkillOfSlot(SLOT_4))
				this->getSkillOfSlot(SLOT_4)->cancelIfPossible();

			Input::rightMouseButton = false;
		}
	}
}

void Player::render(Shader* primitiveShader, Shader* skillShader, Shader* worldSkillShader, TextRenderer* textRenderer, Shader* projectileShader)
{
	
	if (hpBar_shouldRender)
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
			worldSkillShader->activateAlphaBlend();
			if (skill->isActive())
				skill->render(worldSkillShader, skillShader, textRenderer);
			worldSkillShader->deactivateAlphaBlend();
		}
		catch (...){
			std::cerr << "Error rendering entity" << std::endl;
		}
	}
	Entity::render(primitiveShader);
}

/* *********************************** */
/* ********** PRIVATE METHODS ********* */
/* *********************************** */

/* TEXTURE-RELATED METHODS */

void Player::refreshTexture()
{
	double now = LinkedTime::getTime();
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

void Player::moveOnline(Map* map)
{
	if (isMovingTo)
	{
		glm::vec3 pPos = this->getTransform()->getPosition();
		glm::vec3 vec3NewPos = this->ai->getNextStep(this->destination);
		MovementDefinition newPos = this->ai->getMovementDefinitionOfDestination(vec3NewPos, false);

		if (this->ai->reachDestination(newPos.movement, this->destination))
		{
			this->moving = false;
			this->isMovingTo = false;
		}
		else if (map->coordinateHasCollision(newPos.movement))
		{
			this->getTransform()->translate(destination.x, destination.y, destination.z);
			this->moving = false;
			this->isMovingTo = false;
		}
		else
		{
			this->getTransform()->translate(newPos.movement.x, newPos.movement.y, newPos.movement.z);
			this->currentDirection = newPos.direction;
			this->moving = true;
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

glm::vec3 Player::getPlayerNextPosition(MovementDirection direction)
{
	glm::vec3 deltaVec;

	switch (direction)
	{
		case TOP:
			deltaVec = glm::vec3(0, 1, 0); break;
		case TOP_RIGHT:
			deltaVec = glm::vec3(1 / SQRT2, 1 / SQRT2, 0); break;
		case RIGHT:
			deltaVec = glm::vec3(1, 0, 0); break;
		case BOTTOM_RIGHT:
			deltaVec = glm::vec3(1 / SQRT2, -1 / SQRT2, 0); break;
		case BOTTOM:
			deltaVec = glm::vec3(0, -1, 0); break;
		case BOTTOM_LEFT:
			deltaVec = glm::vec3(-1 / SQRT2, -1 / SQRT2, 0); break;
		case LEFT:
			deltaVec = glm::vec3(-1, 0, 0); break;
		case TOP_LEFT:
			deltaVec = glm::vec3(-1 / SQRT2, 1 / SQRT2, 0); break;
		default:
			deltaVec = glm::vec3(0, 0, 0); break;
	}

	glm::vec3 newDestination = this->getTransform()->getPosition() + deltaVec;
	return this->ai->getNextStep(newDestination);
}

/* HPBAR RELATED METHODS */

void Player::hpBarRenderOptions(bool shouldRender)
{
	this->hpBar_shouldRender = shouldRender;
}