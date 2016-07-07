#include "MonsterEndureSkill.h"
#include "PacketController.h"
#include "Game.h"
#include "Player.h"
#include "LinkedTime.h"
#include "Monster.h"

MonsterEndureSkill::MonsterEndureSkill(SkillOwner owner) : Skill(owner)
{
}

MonsterEndureSkill::~MonsterEndureSkill()
{
}

void MonsterEndureSkill::execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId)
{
	if (this->owner == SkillOwner::MONSTER)
	{
		this->active = true;
		Monster* owner = (Monster*)this->getWorldObject();
		glm::vec3 initialAuraPos = owner->getPosition();
		this->skillPhase = 0;
		this->timeReference = LinkedTime::getTime();
		owner->setKnockbackable(false);
		owner->setTotalSpeed(owner->getTotalSpeed() + SPEED_BONUS);
		PacketController::sendMonsterEndureSkill(this->getWorldObject()->getId());
	}
}
void MonsterEndureSkill::update(std::vector<Monster*> *monsters, std::vector<Player*> *players)
{
	if (this->isActive() && this->owner == SkillOwner::MONSTER)
	{
		double now = LinkedTime::getTime();
		Monster* owner = (Monster*)this->getWorldObject();

		if (!owner->isAlive())
		{
			this->active = false;
		}
		else if (this->skillPhase == 16)
		{
			this->active = false;
			owner->setKnockbackable(true);
			owner->setTotalSpeed(owner->getTotalSpeed() - SPEED_BONUS);
		}
		else
		{
			if (now > (this->timeReference + TIME_OFFSET))
			{
				this->skillPhase++;
				this->timeReference = now;
			}
		}
	}
}