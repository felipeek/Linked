#include "SwapSkill.h"
#include "Player.h"
#include "SkillIcon.h"
#include "Game.h"
#include "PacketController.h"

#define SWAP_SKILL_COOLDOWN 5

SwapSkill::SwapSkill(SkillOwner owner, std::vector<Monster*>* monsters, std::vector<Player*>* players, Player** localPlayer) : Skill (owner, monsters, players, localPlayer)
{
	/* SKILL ICON */
	Texture* enabledSkillIconTexture = new Texture("./res/Skills/swap_icon.png");
	Texture* disabledSkillIconTexture = new Texture("./res/Skills/swap_icon_black.png");
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
	if (!this->active && !this->isOnCooldown() && this->status == SwapSkillStatus::IDLE)
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

void SwapSkill::execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId)
{
	this->active = true;
	this->status = SwapSkillStatus::ANIMATION;
}

bool SwapSkill::cancelIfPossible()
{
	return false;
}

void SwapSkill::update()
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
				Player* ownPlayer = ((Player*)entity);
				Player* targetPlayer = ownPlayer->getLink();

				if (targetPlayer != NULL)
				{
					glm::vec3 ownPlayerPosition = ownPlayer->getTransform()->getPosition();
					glm::vec3 targetPlayerPosition = targetPlayer->getTransform()->getPosition();
					ownPlayer->getTransform()->translate(targetPlayerPosition.x, targetPlayerPosition.y, targetPlayerPosition.z);
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