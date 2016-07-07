#include "CureBlessingSkill.h"
#include "PacketController.h"
#include "Game.h"
#include "Player.h"

#define CURE_BLESSING_SKILL_THRESHOLD 3.0f

CureBlessingSkill::CureBlessingSkill(SkillOwner owner) : Skill(owner)
{
	this->status = CureBlessingSkillStatus::IDLE;
	this->cooldown = CURE_BLESSING_SKILL_COOLDOWN;
}

CureBlessingSkill::~CureBlessingSkill()
{
}

void CureBlessingSkill::execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId)
{
	this->status = CureBlessingSkillStatus::EXECUTION;
	this->active = true;

	Player* targetPlayer = PacketController::getPlayerOfClient(targetCreatureId);

	if (targetPlayer != nullptr && this->owner == PLAYER)
		targetPlayer->setHp((unsigned int)round((CURE_BLESSING_SKILL_PERCENTAGE / 100.0f)*(float)targetPlayer->getTotalMaximumHp()));
}
void CureBlessingSkill::update(std::vector<Monster*> *monsters, std::vector<Player*> *players)
{
	if (this->isActive() && this->status != CureBlessingSkillStatus::IDLE)
	{
		if (this->status == CureBlessingSkillStatus::EXECUTION)
		{
			this->active = false;
			this->status = CureBlessingSkillStatus::IDLE;
			this->startCooldownContage();
			this->checkCooldown = true;
		}
	}
	else if (checkCooldown && !this->isOnCooldown())
	{
		this->checkCooldown = false;
	}
}