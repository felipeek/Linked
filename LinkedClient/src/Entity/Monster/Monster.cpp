#include "Monster.h"
#include "MonsterAI.h"
#include "LinkedTime.h"
#include "Primitive.h"
#include "Game.h"
#include "Audio.h"

#define FIRST_ID 1

unsigned short Monster::NEXT_ID = FIRST_ID;

Monster::Monster(Transform* transform, Mesh* mesh, Texture* texture) : Entity(transform, mesh, texture)
{
	this->ai = new MonsterAI(*this);
	this->receiveDamageSound = new Audio("./res/Audio/monster_hit.wav", AudioType::SOUND);
	setName(MONSTER_DEFAULT_NAME);
	setHp(MONSTER_DEFAULT_HP);
	setTotalMaximumHp(MONSTER_DEFAULT_TOTAL_MAX_HP);
	setTotalAttack(MONSTER_DEFAULT_TOTAL_ATTACK);
	setTotalDefense(MONSTER_DEFAULT_TOTAL_DEFENSE);
	setTotalMagicalPower(MONSTER_DEFAULT_TOTAL_MAGICAL_POWER);
	setTotalSpeed(MONSTER_DEFAULT_TOTAL_SPEED);
	setTotalAttackSpeed(MONSTER_DEFAULT_TOTAL_ATTACK_SPEED);
	setTotalRange(MONSTER_DEFAULT_TOTAL_RANGE);

	if (!Game::multiplayer)
		this->id = NEXT_ID++;
}

Monster::~Monster()
{
	delete this->ai;
	delete this->receiveDamageSound;
}

/* *********************************** */
/* ********** PUBLIC METHODS ********* */
/* *********************************** */

/* BASIC ATTRIBUTES */

int Monster::getId()
{
	return this->id;
}

void Monster::setId(int id)
{
	this->id = id;
}

unsigned int Monster::getTextureQuantity()
{
	return this->textureQuantity;
}

void Monster::setTextureQuantity(unsigned int textureQuantity)
{
	this->textureQuantity = textureQuantity;
}

std::string Monster::getName()
{
	return name;
}

void Monster::setName(std::string name)
{
	this->name = name;
}

unsigned int Monster::getHp()
{
	return hp;
}

void Monster::setHp(unsigned int hp)
{
	this->hp = hp;
}

unsigned int Monster::getTotalMaximumHp()
{
	return totalMaximumHp;
}

void Monster::setTotalMaximumHp(unsigned int totalMaximumHp)
{
	this->totalMaximumHp = totalMaximumHp;
}

unsigned int Monster::getTotalAttack()
{
	return totalAttack;
}

void Monster::setTotalAttack(unsigned int totalAttack)
{
	this->totalAttack = totalAttack;
}

unsigned int Monster::getTotalDefense()
{
	return totalDefense;
}

void Monster::setTotalDefense(unsigned int totalDefense)
{
	this->totalDefense = totalDefense;
}

unsigned int Monster::getTotalSpeed()
{
	return totalSpeed;
}

void Monster::setTotalSpeed(unsigned int totalSpeed)
{
	this->totalSpeed = totalSpeed;
}

unsigned int Monster::getTotalRange()
{
	return totalRange;
}

void Monster::setTotalRange(unsigned int totalRange)
{
	this->totalRange = totalRange;
}

unsigned int Monster::getTotalCollisionRange()
{
	return this->totalCollisionRange;
}

void Monster::setTotalCollisionRange(unsigned int totalCollisionRange)
{
	this->totalCollisionRange = totalCollisionRange;
}

unsigned int Monster::getTotalAttackSpeed()
{
	return this->totalAttackSpeed;
}

void Monster::setTotalAttackSpeed(unsigned int totalAttackSpeed)
{
	this->totalAttackSpeed = totalAttackSpeed;
}

unsigned int Monster::getTotalMagicalPower()
{
	return totalMagicalPower;
}

void Monster::setTotalMagicalPower(unsigned int totalMagicalPower)
{
	this->totalMagicalPower = totalMagicalPower;
}

int Monster::getMapColorRed()
{
	return (int)this->mapColor.r;
}

void Monster::setMapColorRed(int red)
{
	this->mapColor.r = (float)red;
}

int Monster::getMapColorGreen()
{
	return (int)this->mapColor.g;
}

void Monster::setMapColorGreen(int green)
{
	this->mapColor.g = (float)green;
}

int Monster::getMapColorBlue()
{
	return (int)this->mapColor.b;
}

void Monster::setMapColorBlue(int blue)
{
	this->mapColor.b = (float)blue;
}

glm::vec3 Monster::getMapColor()
{
	return mapColor;
}

void Monster::setMapColor(glm::vec3 mapColor)
{
	this->mapColor = mapColor;
}

/* STATUS */

bool Monster::isAlive()
{
	return this->alive;
}

void Monster::killMonster()
{
	this->alive = false;
	this->killTime = LinkedTime::getTime();
}

bool Monster::isAttacking()
{
	return this->attacking;
}

void Monster::attack()
{
	this->attacking = true;
	lastAttackTime = LinkedTime::getTime();
}

bool Monster::isReceivingDamage()
{
	return this->receivingDamage;
}

void Monster::receiveDamage()
{
	this->receiveDamageSound->play();
	this->receivingDamage = true;
	lastReceivedDamageTime = LinkedTime::getTime();
}

bool Monster::shouldTranslate()
{
	return this->bTranslate;
}

void Monster::setShouldTranslate(bool bTranslate)
{
	this->bTranslate = bTranslate;
}

void Monster::move(MovementDirection direction)
{
	this->movingDirection = direction;
	this->moving = true;
}

void Monster::stop()
{
	this->moving = false;
}

bool Monster::isMoving()
{
	return this->moving;
}

bool Monster::shouldBeDeleted()
{
	if (!this->isAlive())
	{
		double now = LinkedTime::getTime();
		return (now - killTime) > DEATH_TIME;
	}
	return false;
}

/* COMBAT */

void Monster::doDamage(unsigned int damage)
{
	if (damage >= hp)
	{
		hp = 0;
		this->killMonster();
	}
	else
		hp = hp - damage;

	this->receiveDamage();
}

void Monster::attackCreature(Creature* creature)
{
	unsigned int damage = (unsigned int)ceil(getTotalAttack() / (creature->getTotalDefense() / 10.0f));
	creature->doDamage(damage);
	this->attack();
}

void Monster::action(int actionId, int xid, glm::vec3 vector)
{
}

/* UPDATE & RENDER */

void Monster::update(Map* map, Player* player)
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
}

void Monster::render(Shader* shader)
{
	Entity::render(shader);
}

/* MOVEMENT */

void Monster::startOnlineMovement(glm::vec3 position)
{
}

/* COPY */

Monster* Monster::getCopy(Monster* copy)
{
	// Create new monster if necessary.
	if (copy == nullptr) copy = new Monster(nullptr, nullptr, nullptr);

	// Copy Basic Attributes
	copy->setTextureQuantity(this->getTextureQuantity());
	copy->setTotalAttack(this->getTotalAttack());
	copy->setTotalDefense(this->getTotalDefense());
	copy->setHp(this->getHp());
	copy->setTotalMaximumHp(this->getTotalMaximumHp());
	copy->setMapColor(this->getMapColor());
	copy->setName(this->getName());
	copy->setTotalSpeed(this->getTotalSpeed());
	copy->setTotalRange(this->getTotalRange());
	copy->setTotalCollisionRange(this->getTotalCollisionRange());
	copy->setTotalAttackSpeed(this->getTotalAttackSpeed());
	// Copy Mesh (A new mesh/quad object must be created for each monster)
	if (copy->getMesh() == nullptr)	copy->setMesh(new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f, 7, 7)));
	// Copy Texture (The same texture will be setted for all monsters of same class)
	copy->setTexture(this->getTexture());
	// Copy Transform (A new transform object must be created for each monster)
	Transform *monsterTransform = this->getTransform();
	glm::vec3 monsterTransformPosition = monsterTransform->getPosition();
	glm::vec3 monsterTransformScale = monsterTransform->getScale();
	copy->setTransform(new Transform(monsterTransformPosition, 35, glm::vec3(1,0,0), monsterTransformScale));

	return copy;
}

/* TEXTURE */

void Monster::changeTextureIndex(int index)
{
	this->getMesh()->getQuad()->setIndex(index);
}

int Monster::getCurrentTextureIndex()
{
	return this->getMesh()->getQuad()->getIndex();
}