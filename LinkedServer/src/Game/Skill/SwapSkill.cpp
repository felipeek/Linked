#include "SwapSkill.h"
#include "Player.h"
#include "Game.h"

#define SWAP_SKILL_COOLDOWN 5

SwapSkill::SwapSkill(SkillOwner owner, std::vector<Monster*>* monsters, std::vector<Player*>* players) : Skill(owner, monsters, players)
{
	this->status = SwapSkillStatus::ANIMATION;
	this->cooldown = SWAP_SKILL_COOLDOWN;
}

SwapSkill::~SwapSkill()
{
}

void SwapSkill::execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId)
{
	if (!this->isActive() && !this->isOnCooldown())
	{
		this->active = true;
		this->status = SwapSkillStatus::ANIMATION;
	}
}

void SwapSkill::update()
{
	if (this->active)
	{
		if (this->status == SwapSkillStatus::ANIMATION)
		{
			// TO DO
			this->status = SwapSkillStatus::EXECUTION;
		}
		else if (this->status == SwapSkillStatus::EXECUTION)
		{
			if (this->owner == PLAYER)
			{
				Player* ownPlayer = ((Player*)worldObject);
				Player* targetPlayer = ownPlayer->getLink();

				if (targetPlayer != NULL)
				{
					glm::vec3 ownPlayerPosition = ownPlayer->getPosition();
					glm::vec3 targetPlayerPosition = targetPlayer->getPosition();
					ownPlayer->setPosition(targetPlayerPosition);
					targetPlayer->setPosition(ownPlayerPosition);
				}
			}

			this->active = false;
			this->startCooldownContage();
		}
	}
}