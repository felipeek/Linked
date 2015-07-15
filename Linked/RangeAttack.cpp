#include "RangeAttack.h"
#include "Projectile.h"
#include "Input.h"
#include "Texture.h"
#include "Time.h"
#include "Primitive.h"

#include <iostream>

RangeAttack::RangeAttack(Entity* player, std::vector<Projectile*>* attacks)
{
	this->player = player;
	this->attacks = attacks;
	texture = new Texture("./res/Textures/clownAtlas.png", 2, 2);
	lastTimeUpdate = 0;
	lastTimeCreate = 0;
}


RangeAttack::~RangeAttack()
{
}

void RangeAttack::update()
{
	double now = Time::getTime();
	
	for (int i = 0; i < attacks->size(); i++)
	{
		(*attacks)[i]->update();

		if (now - (*attacks)[i]->spawnTime >= 1.0)
		{
			//delete (*attacks)[i];
			//*attacks->erase(attacks->begin() + i);
			attacks->pop_back();
		}
	}
}

void RangeAttack::attack()
{
	glm::vec3 playerPos = player->getTransform()->getPosition();
	glm::vec3 direction = Input::mouseAttack.attack();
	direction.z = 0;
	direction = glm::normalize(direction);

	double now = Time::getTime();
	std::cout << now - lastTimeCreate << std::endl;

	if (now - lastTimeCreate >= ASPD)
	{
		Mesh* mesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 0.3f, 0.3f));
		Projectile* entityD = new Projectile(new Transform(playerPos + glm::vec3(0,0,playerPos.z), 35, glm::vec3(1, 0, 0), glm::vec3(3, 3, 3)), mesh, texture, speed, direction);
		attacks->push_back(entityD);
		lastTimeCreate = now;
	}
	
}

void RangeAttack::setSpeed(float value)
{
	speed = value;
}

void RangeAttack::setLife(float value)
{
	life = value;
}