#include "TurretMonster.h"
#include "Projectile.h"
#include "Player.h"
#include "LinkedTime.h"
#include "Audio.h"
#include "Game.h"

TurretMonster::TurretMonster(Transform* transform, Mesh* mesh, Texture* texture) : Monster(transform, mesh, texture)
{
	this->projectileMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 0.3f, 0.3f));
	projectileMesh->setReferenceCount(1);
	this->projectileTexture = new Texture("./res/Textures/enemyBall.png");
	projectileTexture->setReferenceCount(1);
	this->attackSound = new Audio("./res/Audio/enemy_attack.wav", AudioType::SOUND);
}

TurretMonster::~TurretMonster()
{
	delete this->projectileMesh;
	delete this->projectileTexture;
	delete this->attackSound;
}

void TurretMonster::update(Map* map, Player* player)
{
	Monster::update(map, player);
	if (this->isAlive())
	{
		if (!Game::multiplayer) this->tryToCreateProjectile(player);
		this->changeTextureIndex(0);
	}
	else
		this->changeTextureIndex(1);

	for (Projectile* p : this->projectiles)
		p->update(map, nullptr, player, true);

	for (unsigned int i = 0; i < this->projectiles.size(); i++)
	{
		if ((this->projectiles)[i]->shouldBeDeleted())
		{
			delete (this->projectiles)[i];
			this->projectiles.erase(this->projectiles.begin() + i);
		}
	}
}

void TurretMonster::render(Shader* shader)
{
	Monster::render(shader);

	for (Projectile* p : this->projectiles)
		p->render(shader);
}

void TurretMonster::startOnlineMovement(glm::vec3 position)
{

}

void TurretMonster::action(int actionId, int xid, glm::vec3 vector)
{
	switch (actionId)
	{
		// Create Projectile Action
		case 0: this->createProjectile(vector, xid); break;
		// Destroy Projectile Action
		case 1: this->destroyProjectile(xid); break;
		default: break;
	}
}

Monster* TurretMonster::getCopy(Monster* copy)
{
	// Create new monster if necessary.
	if (copy == nullptr) copy = new TurretMonster(nullptr, nullptr, nullptr);

	// Copy Mesh (A new mesh/quad object must be created for each monster)
	if (copy->getMesh() == nullptr)	copy->setMesh(new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f, 2, 0)));

	return Monster::getCopy(copy);
}

void TurretMonster::tryToCreateProjectile(Player* player)
{
	double now = LinkedTime::getTime();

	glm::vec3 playerPosition = player->getTransform()->getPosition();
	glm::vec3 monsterPosition = this->getTransform()->getPosition();

	if ((now - lastProjectileTime) > TURRET_MONSTER_PROJECTILE_TIME && player->isAlive() && glm::length(monsterPosition - playerPosition) < TURRET_MONSTER_PROJECTILE_RANGE)
	{
		this->lastProjectileTime = now;
		if (playerPosition.x != monsterPosition.x || playerPosition.y != monsterPosition.y)
		{
			glm::vec3 directionVector = glm::normalize(playerPosition - monsterPosition);
			this->createProjectile(directionVector, 0);
		}
	}
}

void TurretMonster::createProjectile(glm::vec3 direction, int projId)
{
	glm::vec3 monsterPos = this->getTransform()->getPosition();
	monsterPos.z = 0;
	direction.z = 0.0f;

	if (direction.x == 0 && direction.y == 0 && direction.z == 0)
		return;

	Transform* projectileTransform = new Transform(monsterPos + glm::vec3(0, 0, this->getTransform()->getPosition().z), 35, glm::vec3(1, 0, 0), glm::vec3(3, 3, 3));
	Projectile* entityD = new Projectile(projectileTransform, projectileMesh, projectileTexture, direction, ProjectileType::MONSTER_ATTACK, this->getId());
	entityD->setSpeed(TURRET_MONSTER_PROJECTILE_SPEED);
	entityD->setDistance(TURRET_MONSTER_PROJECTILE_RANGE);
	entityD->setPower(this->getTotalAttack());
	entityD->setId(projId);
	projectiles.push_back(entityD);

	this->attackSound->play();
}

void TurretMonster::destroyProjectile(int projId)
{
	for (unsigned int i = 0; i < this->projectiles.size(); i++)
	{
		if ((this->projectiles)[i]->getId() == projId)
		{
			delete (this->projectiles)[i];
			this->projectiles.erase(this->projectiles.begin() + i);
		}
	}
}