#include "LinkSkill.h"
#include "Primitive.h"
#include "Input.h"
#include "SkillIcon.h"
#include "Player.h"
#include "PacketController.h"
#include "Game.h"
#include "Cursor.h"

LinkSkill::LinkSkill(SkillOwner owner) : Skill(owner)
{
	/* AIM ENTITY */
	Mesh* aimMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 0.2f, 0.2f, 2, 0));
	Transform* aimTransform = new Transform(glm::vec3(0, 0, 0), glm::vec3(0.5f , 0.5f, 0.5f));
	Texture* aimTexture = new Texture("./res/Skills/small_aim.png");
	this->aimEntity = new Entity(aimTransform, aimMesh, aimTexture);

	/* SKILL ICON */
	Texture* enabledSkillIconTexture = new Texture(LINK_SKILL_ICON_ENABLED);
	Texture* disabledSkillIconTexture = new Texture(LINK_SKILL_ICON_DISABLED);
	this->skillIcon = new SkillIcon(enabledSkillIconTexture, disabledSkillIconTexture, SLOT_1);

	this->cursorRot = 0;
}

LinkSkill::~LinkSkill()
{
	if (this->aimEntity != NULL)
		delete this->aimEntity;
}

void LinkSkill::prepareExecution(MovementDirection skillDirection)
{
	if (this->owner == PLAYER)
	{
		Player* owner = (Player*)this->getEntity();
		if (!this->active && !owner->hasLink())
		{
			this->active = true;
			this->status = LinkSkillStatus::AIM;
			Game::cursor->hideCursor();
		}
	}
}

void LinkSkill::execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId)
{
	if (this->owner == PLAYER)
	{
		Player* owner = (Player*)this->getEntity();
		Player* targetPlayer = PacketController::getPlayerOfClient(targetCreatureId);

		if (targetPlayer != nullptr && !owner->hasLink() && owner->isAlive())
		{
			this->status = LinkSkillStatus::EXECUTION;
			this->skillIcon->disableIcon();
			this->active = true;
			owner->setLink(targetPlayer);
			targetPlayer->setLink((Player*)(this->entity));
		}
		else
			this->active = false;
	}
	else
		this->active = false;
}

void LinkSkill::sendExecutionToServer(glm::vec3 mousePos, Player* targetPlayer)
{
	if (this->owner == PLAYER)
	{
		Player* owner = (Player*)this->getEntity();

		if (targetPlayer != nullptr && !owner->hasLink() && owner->isAlive())
			PacketController::sendSkillToServer(this->getSlot(), TOP, mousePos, targetPlayer->getClientId());
		else
			this->active = false;
	}
	else
		this->active = false;
}

bool LinkSkill::cancelIfPossible()
{
	if (this->isActive() && this->status == LinkSkillStatus::AIM)
	{
		this->active = false;
		Game::cursor->showCursor();
		return true;
	}
	return false;
}

void LinkSkill::update(std::vector<Monster*> *monsters, std::vector<Player*> *players, Player* localPlayer)
{
	if (this->isActive())
	{
		if (this->status == LinkSkillStatus::AIM)
		{
			Player* targetPlayer = this->getTargetPlayer(players);
			// mouse position related to window, not the world
			glm::vec2 screenPos = Input::mouseAttack.getOrthoCoords();
			glm::vec3 mousePos = glm::vec3(screenPos.x, screenPos.y, 0);
			this->aimEntity->getTransform()->translate(mousePos.x, mousePos.y, 0);
			this->aimEntity->getTransform()->rotate(cursorRot, glm::vec3(0, 0, 1));
			cursorRot += CURSOR_ROTATION_SPEED;

			if (targetPlayer != nullptr)
				this->aimEntity->getMesh()->getQuad()->setIndex(1);
			else
				this->aimEntity->getMesh()->getQuad()->setIndex(0);

			if (Input::attack)
			{
				Game::cursor->showCursor();
				if (targetPlayer != NULL)
				{
					if (Game::multiplayer)
						this->sendExecutionToServer(mousePos, targetPlayer);
					else
						this->execute(TOP, mousePos, targetPlayer->getClientId());
				}
				else
				{
					this->active = false;
				}
			}
		}
		else if (this->status == LinkSkillStatus::EXECUTION)
		{
			this->active = false;
		}
	}
	else
	{
		if (this->owner == PLAYER)
		{
			Player* owner = (Player*)this->getEntity();
			if (owner->hasLink() && this->skillIcon->isEnabled())
				this->skillIcon->disableIcon();
		}
	}
}

void LinkSkill::render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer)
{
	if (this->status == LinkSkillStatus::AIM)
		this->aimEntity->render(skillShader);
}

Player* LinkSkill::getTargetPlayer(std::vector<Player*> *players)
{
	// mouse position related to the world, not the window
	glm::vec3 mousePos = Input::mouseAttack.getMouseIntersection();
	Player* mostClosePlayer = nullptr;
	float currentLength = LINK_SKILL_AIM_THRESHOLD;

	for (Player* player : *(players))
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
		if (currentLength < LINK_SKILL_AIM_THRESHOLD)
			return mostClosePlayer;
	
	return nullptr;
}