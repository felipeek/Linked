#include "TurretMonster.h"
#include "Projectile.h"
#include "Player.h"
#include "LinkedTime.h"

TurretMonster::TurretMonster(Transform* transform, Mesh* mesh, Texture* texture) : Monster(transform, mesh, texture)
{
	this->projectileMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 0.3f, 0.3f));
	projectileMesh->setReferenceCount(1);
	this->projectileTexture = new Texture("./res/Textures/energyBall.png");
	projectileTexture->setReferenceCount(1);
}

TurretMonster::~TurretMonster()
{
	delete this->projectileMesh;
	delete this->projectileTexture;
}

void TurretMonster::update(Map* map, Player* player)
{
	Monster::update(map, player);
	if (this->isAlive())
	{
		this->tryToCreateProjectile(player);
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

void TurretMonster::action(glm::vec3 vector)
{

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
	//bool shouldCreateProjectile = PROJECTILE_CHANCE > (float)(rand() % 10000)/(float)100;
	glm::vec3 playerPosition = player->getTransform()->getPosition();
	glm::vec3 monsterPosition = this->getTransform()->getPosition();

	if ((now - lastProjectileTime) > PROJECTILE_TIME)
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
	Projectile* entityD = new Projectile(projectileTransform, projectileMesh, projectileTexture, 0.4f , direction, ProjectileType::MONSTER_ATTACK);
	entityD->setId(projId);
	projectiles.push_back(entityD);
}