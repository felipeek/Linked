#include "CureBlessingSkill.h"
#include "Primitive.h"
#include "SkillIcon.h"
#include "PacketController.h"
#include "Game.h"
#include "Cursor.h"
#include "Input.h"
#include "Player.h"

#define CURE_BLESSING_SKILL_THRESHOLD 3.0f

CureBlessingSkill::CureBlessingSkill(SkillOwner owner) : Skill(owner)
{
	/* AIM ENTITY */
	Mesh* aimMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 0.2f, 0.2f));
	Transform* aimTransform = new Transform(glm::vec3(0, 0, 0), glm::vec3(0.5f, 0.5f, 0.5f));
	Texture* aimTexture = new Texture("./res/Skills/small_aim.png");
	this->aimEntity = new Entity(aimTransform, aimMesh, aimTexture);

	/* SKILL ICON */
	Texture* enabledSkillIconTexture = new Texture("./res/Skills/cures_blessing.png");
	Texture* disabledSkillIconTexture = new Texture("./res/Skills/cures_blessing_black.png");
	this->skillIcon = new SkillIcon(enabledSkillIconTexture, disabledSkillIconTexture, SLOT_1);

	this->status = CureBlessingSkillStatus::IDLE;
	this->cooldown = CURE_BLESSING_SKILL_COOLDOWN;
	this->cursorRot = 0;
}

CureBlessingSkill::~CureBlessingSkill()
{
	if (this->aimEntity != NULL)
		delete this->aimEntity;
}

void CureBlessingSkill::render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer)
{
	if (this->status == CureBlessingSkillStatus::AIM)
		this->aimEntity->render(skillShader);
}
void CureBlessingSkill::prepareExecution(MovementDirection skillDirection)
{
	if (!this->active && !this->isOnCooldown() && this->status == CureBlessingSkillStatus::IDLE)
	{
		this->active = true;
		this->status = CureBlessingSkillStatus::AIM;
		Game::cursor->hideCursor();
	}
}
void CureBlessingSkill::execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId)
{
	this->status = CureBlessingSkillStatus::EXECUTION;
	this->skillIcon->disableIcon();
	this->active = true;

	Player* targetPlayer = PacketController::getPlayerOfClient(targetCreatureId);

	if (targetPlayer != nullptr && this->owner == PLAYER)
		targetPlayer->setHp((unsigned int)round((CURE_BLESSING_SKILL_PERCENTAGE/100.0f)*(float)targetPlayer->getTotalMaximumHp()));
}
bool CureBlessingSkill::cancelIfPossible()
{
	if (this->isActive() && this->status == CureBlessingSkillStatus::AIM)
	{
		this->active = false;
		Game::cursor->showCursor();
		this->status = CureBlessingSkillStatus::IDLE;
		return true;
	}
	return false;
}
void CureBlessingSkill::update(std::vector<Monster*> *monsters, std::vector<Player*> *players, Player* localPlayer)
{
	if (this->isActive() && this->status != CureBlessingSkillStatus::IDLE)
	{
		if (this->status == CureBlessingSkillStatus::AIM)
		{
			// mouse position related to window, not the world
			glm::vec2 screenPos = Input::mouseAttack.getOrthoCoords();
			glm::vec3 mousePos = glm::vec3(screenPos.x, screenPos.y, 0);
			this->aimEntity->getTransform()->translate(mousePos.x, mousePos.y, 0);
			this->aimEntity->getTransform()->rotate(cursorRot, glm::vec3(0, 0, 1));
			cursorRot += CURSOR_ROTATION_SPEED;

			if (Input::attack)
			{
				Game::cursor->showCursor();
				Player* targetPlayer = this->getTargetPlayer(localPlayer, players);
				if (targetPlayer != NULL && targetPlayer->isAlive() == false)
				{
					if (Game::multiplayer)
						PacketController::sendSkillToServer(this->getSlot(), TOP, mousePos, targetPlayer->getClientId());
					else
						this->execute(TOP, mousePos, targetPlayer->getClientId());
				}
				else
				{
					this->active = false;
					this->status = CureBlessingSkillStatus::IDLE;
				}
			}
		}
		else if (this->status == CureBlessingSkillStatus::WAITING_FOR_SERVER_RESPONSE)
		{

		}
		else if (this->status == CureBlessingSkillStatus::EXECUTION)
		{
			this->active = false;
			this->status = CureBlessingSkillStatus::IDLE;
			this->startCooldownContage();
			this->checkCooldown = true;
			this->skillIcon->disableIcon();
		}
	}
	else if (checkCooldown && !this->isOnCooldown())
	{
		this->skillIcon->enableIcon();
		this->checkCooldown = false;
	}
}

Player* CureBlessingSkill::getTargetPlayer(Player* localPlayer, std::vector<Player*> *players)
{
	// mouse position related to the world, not the window
	glm::vec3 mousePos = Input::mouseAttack.getMouseIntersection();
	Player* mostClosePlayer = localPlayer;
	float currentLength = glm::length(glm::vec2(mousePos.x, mousePos.y) - glm::vec2(localPlayer->getTransform()->getPosition().x, localPlayer->getTransform()->getPosition().y));

	for (Player* player : *players)
	{
		glm::vec3 playerPosition = player->getTransform()->getPosition();
		float length = glm::length(mousePos - playerPosition);

		if (length < currentLength)
		{
			mostClosePlayer = player;
			currentLength = length;
		}
	}

	if (mostClosePlayer != nullptr)
		if (currentLength < CURE_BLESSING_SKILL_THRESHOLD)
			return mostClosePlayer;

	return nullptr;
}