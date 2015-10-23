#include "HoshoyoExplosionSkill.h"
#include "Monster.h"
#include "Game.h"
#include "PacketController.h"

HoshoyoExplosionSkill::HoshoyoExplosionSkill(std::vector<Monster*>* monsters) : Skill(monsters)
{
	this->skillPhase = 0;
}

HoshoyoExplosionSkill::~HoshoyoExplosionSkill()
{
}

void HoshoyoExplosionSkill::update()
{
	if (this->isActive())
	{
		if (this->skillPhase % 27 == 0)
			this->hitEnemiesOnSkillRadius();
		if (this->skillPhase == 81)
		{
			this->active = false;
		}
		else
		{
			this->skillPhase++;
		}
	}
}

void HoshoyoExplosionSkill::execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId)
{
	if (!this->active)
	{
		explosionPosition = skillTargetPosition;
		this->skillPhase = 0;
		this->active = true;
	}
}

const float skillRadius = 10.0f;
const int skillDamage = 10;

void HoshoyoExplosionSkill::hitEnemiesOnSkillRadius()
{
	for (Monster* monster : *(this->monsters))
	{
		glm::vec3 diffVector = monster->getPosition() - explosionPosition;
		if (glm::length(diffVector) < skillRadius && monster->isAlive())
		{
			monster->doDamage(skillDamage);
			PacketController::queueMonsterDamage(monster->getId(), -1, skillDamage);
		}
	}
}