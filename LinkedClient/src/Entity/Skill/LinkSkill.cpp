#include "LinkSkill.h"
#include "Primitive.h"
#include "Input.h"
#include "SkillIcon.h"
#include "Player.h"
#include "PacketController.h"
#include "Game.h"
#include "Cursor.h"

#define LINK_SKILL_THRESHOLD 3.0f

LinkSkill::LinkSkill(SkillOwner owner) : Skill(owner)
{
	/* AIM ENTITY */
	Mesh* aimMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 0.2f, 0.2f));
	Transform* aimTransform = new Transform(glm::vec3(0, 0, 0), glm::vec3(0.5f , 0.5f, 0.5f));
	this->aimRedTexture = new Texture("./res/Skills/small_aim_red.png");
	this->aimBlackTexture = new Texture("./res/Skills/small_aim_black.png");
	this->aimEntity = new Entity(aimTransform, aimMesh, aimBlackTexture);

	/* SKILL ICON */
	Texture* enabledSkillIconTexture = new Texture("./res/Skills/link_icon.png");
	Texture* disabledSkillIconTexture = new Texture("./res/Skills/link_icon_black.png");
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
		bool linked = ((Player*)this->getEntity())->getLink() != nullptr;
		if (!this->active && !linked)
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
		bool linked = ((Player*)this->getEntity())->getLink() != nullptr;
		this->status = LinkSkillStatus::EXECUTION;
		this->skillIcon->disableIcon();
		this->active = true;

		Player* targetPlayer = PacketController::getPlayerOfClient(targetCreatureId);

		if (targetPlayer != nullptr && !linked)
		{
			((Player*)(this->entity))->setLink(targetPlayer);
			targetPlayer->setLink((Player*)(this->entity));
		}
	}
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
			// mouse position related to window, not the world
			glm::vec2 screenPos = Input::mouseAttack.getOrthoCoords();
			glm::vec3 mousePos = glm::vec3(screenPos.x, screenPos.y, 0);
			this->aimEntity->getTransform()->translate(mousePos.x, mousePos.y, 0);
			this->aimEntity->getTransform()->rotate(cursorRot, glm::vec3(0, 0, 1));
			cursorRot += CURSOR_ROTATION_SPEED;

			if (this->getTargetPlayer(players) != nullptr)
				this->aimEntity->setTexture(this->aimRedTexture);
			else
				this->aimEntity->setTexture(this->aimBlackTexture);

			if (Input::attack)
			{
				Game::cursor->showCursor();
				Player* targetPlayer = this->getTargetPlayer(players);
				if (targetPlayer != NULL)
				{
					if (Game::multiplayer)
						PacketController::sendSkillToServer(this->getSlot(), TOP, mousePos, targetPlayer->getClientId());
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
	float currentLength = LINK_SKILL_THRESHOLD;

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
		if (currentLength < LINK_SKILL_THRESHOLD)
			return mostClosePlayer;
	
	return nullptr;
}