#include "SwapSkill.h"
#include "Player.h"
#include "SkillIcon.h"
#include "Game.h"
#include "PacketController.h"

#define SWAP_SKILL_COOLDOWN 5

SwapSkill::SwapSkill(SkillOwner owner) : Skill (owner)
{
	/* SKILL ICON */
	Texture* enabledSkillIconTexture = new Texture(SWAP_SKILL_ICON_ENABLED);
	Texture* disabledSkillIconTexture = new Texture(SWAP_SKILL_ICON_DISABLED);
	this->skillIcon = new SkillIcon(enabledSkillIconTexture, disabledSkillIconTexture, SLOT_1);

	this->status = SwapSkillStatus::IDLE;
	this->cooldown = SWAP_SKILL_COOLDOWN;
}

SwapSkill::~SwapSkill()
{
}

void SwapSkill::render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer)
{

}

void SwapSkill::prepareExecution(MovementDirection skillDirection)
{
	if (this->owner == PLAYER)
	{
		Player* owner = (Player*)this->getEntity();

		if (owner->isAlive() && owner->hasLink() && !this->active && !this->isOnCooldown() && this->status == SwapSkillStatus::IDLE)
		{
			if (Game::multiplayer)
			{
				this->status = SwapSkillStatus::WAITING_FOR_SERVER_RESPONSE;
				PacketController::sendSkillToServer(this->getSlot(), skillDirection, glm::vec3(0, 0, 0), 0);
			}
			else
				execute(skillDirection, glm::vec3(0, 0, 0), 0);
		}
	}
}

void SwapSkill::execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId)
{
	this->active = true;
	this->status = SwapSkillStatus::ANIMATION;
}

bool SwapSkill::cancelIfPossible()
{
	return false;
}

void SwapSkill::update(std::vector<Monster*> *monsters, std::vector<Player*> *players, Player* localPlayer)
{
	if (this->active && this->status != SwapSkillStatus::IDLE)
	{
		if (this->status == SwapSkillStatus::WAITING_FOR_SERVER_RESPONSE)
		{

		}
		else if (this->status == SwapSkillStatus::ANIMATION)
		{
			// TO DO
			this->status = SwapSkillStatus::EXECUTION;
		}
		else if (this->status == SwapSkillStatus::EXECUTION)
		{
			if (this->owner == PLAYER)
			{
				Player* owner = ((Player*)entity);
				Player* targetPlayer = owner->getLink();

				if (targetPlayer != NULL && owner->isAlive() && targetPlayer->isAlive())
				{
					glm::vec3 ownPlayerPosition = owner->getTransform()->getPosition();
					glm::vec3 targetPlayerPosition = targetPlayer->getTransform()->getPosition();
					owner->getTransform()->translate(targetPlayerPosition.x, targetPlayerPosition.y, targetPlayerPosition.z);
					targetPlayer->getTransform()->translate(ownPlayerPosition.x, ownPlayerPosition.y, ownPlayerPosition.z);
				}
			}

			this->active = false;
			this->startCooldownContage();
			this->checkCooldown = true;
			this->skillIcon->disableIcon();
			this->status = SwapSkillStatus::IDLE;
		}
	}
	else if (checkCooldown && !this->isOnCooldown())
	{
		this->skillIcon->enableIcon();
		this->checkCooldown = false;
	}
}