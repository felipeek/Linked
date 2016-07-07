#include "MonsterExplosionSkill.h"
#include "PacketController.h"
#include "Game.h"
#include "Player.h"

MonsterExplosionSkill::MonsterExplosionSkill(SkillOwner owner) : Skill(owner)
{
}

MonsterExplosionSkill::~MonsterExplosionSkill()
{
}

void MonsterExplosionSkill::execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId)
{
	this->active = true;
	this->skillPhase = 0;
	PacketController::sendMonsterExplosionSkill(this->getWorldObject()->getId());
}

void MonsterExplosionSkill::update(std::vector<Monster*> *monsters, std::vector<Player*> *players)
{
	if (this->isActive())
	{
		if (this->skillPhase < 12)
			this->skillPhase++;
		else if (this->skillPhase >= 12)
		{
			this->active = false;
			this->skillPhase = 0;
		}
	}
}