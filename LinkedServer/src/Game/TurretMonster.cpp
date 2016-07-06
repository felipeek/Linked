#include "TurretMonster.h"
#include "Projectile.h"
#include "Player.h"
#include "LinkedTime.h"
#include "PacketController.h"
#include "MonsterAI.h"

TurretMonster::TurretMonster() : Monster()
{

}

TurretMonster::~TurretMonster()
{

}

void TurretMonster::update(Map* map, std::vector<Player*>* players, std::vector<Monster*>* monsters)
{
	Monster::update(map, players, monsters);
	Player* player = this->ai->findClosestWorldObjectToAttack(map, players);
	if (this->isAlive() && player != nullptr)
		this->tryToCreateProjectile(player);
}

Monster* TurretMonster::getCopy(Monster* copy)
{
	// Create new monster if necessary.
	if (copy == nullptr) copy = new TurretMonster();

	return Monster::getCopy(copy);
}

void TurretMonster::tryToCreateProjectile(Player* player)
{
	double now = LinkedTime::getTime();

	glm::vec3 playerPosition = player->getPosition();
	glm::vec3 monsterPosition = this->getPosition();

	if ((now - lastProjectileTime) > TURRET_MONSTER_PROJECTILE_TIME && player->isAlive() && glm::length(monsterPosition - playerPosition) < TURRET_MONSTER_PROJECTILE_RANGE)
	{
		this->lastProjectileTime = now;
		if (playerPosition.x != monsterPosition.x || playerPosition.y != monsterPosition.y)
		{
			glm::vec3 directionVector = glm::normalize(playerPosition - monsterPosition);
			this->createProjectile(directionVector, 0, player->getId());
		}
	}
}

void TurretMonster::createProjectile(glm::vec3 direction, int projId, int playerId)
{
	glm::vec3 monsterPos = this->getPosition();
	monsterPos.z = 0;
	direction.z = 0.0f;

	if (direction.x == 0 && direction.y == 0 && direction.z == 0)
		return;

	Projectile* monsterProjectile = new Projectile(monsterPos + glm::vec3(0, 0, this->getPosition().z), direction, playerId);
	PacketController::sendProjectileAttackByTurretMonster(this->getId(), monsterProjectile);
	delete monsterProjectile;
}

/* NETWORK MOVEMENT */

bool TurretMonster::mustUpdateDestinationToClients()
{
	return false;
}

glm::vec3 TurretMonster::getDestination()
{
	return this->getPosition();
}