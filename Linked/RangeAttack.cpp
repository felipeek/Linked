#include "RangeAttack.h"
#include "Projectile.h"
#include "Input.h"
#include "Texture.h"

RangeAttack::RangeAttack(Entity* player, std::vector<Projectile*>* attacks)
{
	this->player = player;
	this->attacks = attacks;
}


RangeAttack::~RangeAttack()
{
}

void RangeAttack::update()
{
	for (Projectile* p : *attacks)
	{
		p->update();
	}
}

void RangeAttack::attack()
{
	glm::vec3 playerPos = player->getTransform()->getPosition();
	glm::vec3 direction = Input::mouseAttack.attack();
	direction.z = playerPos.z;
	direction = glm::normalize(direction);

	Texture* textureD = new Texture("./res/Textures/clownAtlas.png", 2, 2);
	Projectile* entityD = new Projectile(new Transform(playerPos, 35, glm::vec3(1, 0, 0), glm::vec3(10, 10, 10)), player->getMesh(), textureD, speed, direction);
	attacks->push_back(entityD);
}

void RangeAttack::setSpeed(float value)
{
	speed = value;
}

void RangeAttack::setLife(float value)
{
	life = value;
}