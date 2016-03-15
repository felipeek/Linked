#include "RangeAttack.h"
#include "Projectile.h"
#include "Input.h"
#include "Texture.h"
#include "LinkedTime.h"
#include "Primitive.h"
#include "Monster.h"
#include "Map.h"
#include "Mesh.h"
#include "Player.h"
#include "PacketController.h"
#include "Game.h"

RangeAttack::RangeAttack(Player* player, std::vector<Projectile*>* attacks)
{
	mesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 0.3f, 0.3f));
	mesh->setReferenceCount(1);
	this->player = player;
	this->attacks = attacks;
	texture = new Texture("./res/Textures/energyBall.png");
	texture->setReferenceCount(1);
	lastTimeUpdate = 0;
	lastTimeCreate = 0;
}

RangeAttack::~RangeAttack()
{
	delete texture;
	delete mesh;
}

std::vector<Projectile*>* RangeAttack::getAttacks()
{
	return this->attacks;
}

void RangeAttack::createProjectile(glm::vec3 direction, int projId)
{
	glm::vec3 playerPos = player->getTransform()->getPosition();
	playerPos.z = 0;
	direction.z = 0.0f;

	if (direction.x == 0 && direction.y == 0 && direction.z == 0)
		return;

	Transform* projectileTransform = new Transform(playerPos + glm::vec3(0, 0, playerPos.z), 35, glm::vec3(1, 0, 0), glm::vec3(3, 3, 3));
	Projectile* entityD = new Projectile(projectileTransform, mesh, texture, direction, ProjectileType::PLAYER_ATTACK);
	entityD->setSpeed(PLAYER_PROJECTILE_SPEED);
	entityD->setDistance(PLAYER_PROJECTILE_DISTANCE);
	entityD->setPower(this->player->getTotalAttack());
	entityD->setId(projId);
	(*attacks).push_back(entityD);

	//std::cout << "created projectile " << projId << "." << std::endl;
}

bool RangeAttack::createProjectileDirectedToMouse()
{
	double now = LinkedTime::getTime();

	if (now - lastTimeCreate >= PLAYER_ASPD)
	{
		glm::vec3 playerPos = player->getTransform()->getPosition();
		glm::vec3 direction = Input::mouseAttack.getMouseIntersection() - playerPos;

		this->createProjectile(direction, 0);

		lastTimeCreate = now;

		return true;
	}

	return false;
}

void RangeAttack::sendAttackToServer()
{
	double now = LinkedTime::getTime();

	if (now - lastTimeCreate >= PLAYER_ASPD)
	{
		glm::vec3 playerPos = player->getTransform()->getPosition();
		glm::vec3 direction = Input::mouseAttack.getMouseIntersection() - playerPos;

		PacketController::sendAttackToServer(direction);

		lastTimeCreate = now;
	}
}

void RangeAttack::update(Map* map, std::vector<Monster*>* monsters)
{
	for (unsigned int i = 0; i < attacks->size(); i++)
		(*attacks)[i]->update(map, monsters, nullptr, true);

	for (unsigned int i = 0; i < attacks->size(); i++)
	{
		if ((*attacks)[i]->shouldBeDeleted())
		{
			delete (*attacks)[i];
			attacks->erase((*attacks).begin() + i);
		}
	}
}

void RangeAttack::input()
{
	if (Input::attack && player->isAlive())
	{
		if (Game::multiplayer)
		{
			this->sendAttackToServer();
		}
		else
		{
			if (createProjectileDirectedToMouse()) player->attack();
		}
	}
}
