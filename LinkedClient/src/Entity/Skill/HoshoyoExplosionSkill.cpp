#include "HoshoyoExplosionSkill.h"
#include "Primitive.h"
#include "Input.h"
#include "SkillIcon.h"
#include "Monster.h"
#include "Game.h"
#include "Cursor.h"
#include "PacketController.h"
#include "TextRenderer.h"

// TODO: Skill Animation flow must be implemented using LinkedTime, not only the update call.

HoshoyoExplosionSkill::HoshoyoExplosionSkill(SkillOwner owner, std::vector<Monster*>* monsters, std::vector<Player*>* players, Player** localPlayer) : Skill(owner, monsters, players, localPlayer)
{
	/* AIM ENTITY */
	Mesh* aimMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f, 1, 12));
	Transform* aimTransform = new Transform(glm::vec3(0, 0, 0.1f), 0, glm::vec3(1, 0, 0), glm::vec3(10, 10, 10));
	Texture* aimTexture = new Texture("./res/Skills/aim.png");
	this->aimEntity = new Entity(aimTransform, aimMesh, aimTexture);
	this->cursorRot = 0;

	/* RANGE ENTITY */
	Mesh* rangeMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f, 1, 12));
	Transform* rangeTransform = new Transform(glm::vec3(0, 0, 0.1f), 0, glm::vec3(1, 0, 0), glm::vec3(HOSHOYO_EXPLOSION_SKILL_MAX_RADIUS, HOSHOYO_EXPLOSION_SKILL_MAX_RADIUS, HOSHOYO_EXPLOSION_SKILL_MAX_RADIUS));
	Texture* rangeTexture = new Texture("./res/Skills/range.png");
	this->rangeEntity = new Entity(rangeTransform, rangeMesh, rangeTexture);

	/* EXPLOSION ENTITY (THIS) */
	Mesh* hoshoyoExplosionMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f, 9, 12));
	Transform* hoshoyoExplosionTransform = new Transform(glm::vec3(520, 500, 1.5f), 0, glm::vec3(1, 0, 0), glm::vec3(10, 10, 10));
	Texture* hoshoyoExplosionTexture = new Texture("./res/Skills/hoshoyoexplosion.png");
	this->setMesh(hoshoyoExplosionMesh);
	this->setTransform(hoshoyoExplosionTransform);
	this->setTexture(hoshoyoExplosionTexture);

	/* SKILL ICON */
	Texture* enabledSkillIconTexture = new Texture("./res/Skills/hoshoyoexplosion_icon.png");
	Texture* disabledSkillIconTexture = new Texture("./res/Skills/hoshoyoexplosion_icon_black.png");
	this->skillIcon = new SkillIcon(enabledSkillIconTexture, disabledSkillIconTexture, SLOT_1);
}

HoshoyoExplosionSkill::~HoshoyoExplosionSkill()
{
	if (this->aimEntity != NULL)
		delete this->aimEntity;
	if (this->rangeEntity != NULL)
		delete this->rangeEntity;
}

void HoshoyoExplosionSkill::render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer)
{
	if (this->status == HoshoyoExplosionSkillStatus::AIM)
	{
		this->aimEntity->render(primitiveShader);
		this->rangeEntity->render(primitiveShader);
	}
	else if (this->status == HoshoyoExplosionSkillStatus::EXECUTION)
	{
		Entity::render(primitiveShader);
	}
}

void HoshoyoExplosionSkill::prepareExecution(MovementDirection skillDirection)
{
	if (!this->active)
	{
		this->active = true;
		this->status = HoshoyoExplosionSkillStatus::AIM;
		Game::cursor->hideCursor();
	}
}

void HoshoyoExplosionSkill::update()
{
	if (this->isActive())
	{
		if (this->status == HoshoyoExplosionSkillStatus::AIM)
		{
			// mouse position related to the world, not the window
			glm::vec3 mousePos = Input::mouseAttack.getMouseIntersection();
			glm::vec3 entityPos = this->entity->getTransform()->getPosition();
			float diff = glm::length(mousePos - entityPos);
			if (diff < (float)(HOSHOYO_EXPLOSION_SKILL_MAX_RADIUS - 1))
			{
				this->aimEntity->getTransform()->translate(mousePos.x, mousePos.y, 0.1f);
				Game::cursor->hideCursor();
			}
			else
			{
				glm::vec3 v = (float)(HOSHOYO_EXPLOSION_SKILL_MAX_RADIUS - 1)*glm::normalize(mousePos - entityPos);
				this->aimEntity->getTransform()->translate((entityPos+v).x, (entityPos + v).y, 0.1f);
				Game::cursor->showCursor();
			}
			this->aimEntity->getTransform()->rotate(cursorRot, glm::vec3(0, 0, 1));
			cursorRot += CURSOR_ROTATION_SPEED;
			this->rangeEntity->getTransform()->translate(entityPos.x, entityPos.y, 0.1f);

			if (Input::attack)
			{
				Game::cursor->showCursor();
				if (Game::multiplayer)
				{
					PacketController::sendSkillToServer(this->getSlot(), TOP, this->aimEntity->getTransform()->getPosition(), 0);
				}
				else
				{
					this->execute(TOP, this->aimEntity->getTransform()->getPosition(), 0);
				}
			}
		}
		else if (this->status == HoshoyoExplosionSkillStatus::EXECUTION)
		{
			if (!Game::multiplayer)
			{
				if (this->currentExplosionTextureIndex % 27 == 0)
					this->hitEnemiesOnSkillRadius();
			}
			if (this->currentExplosionTextureIndex == 81)
			{
				this->active = false;
				this->skillIcon->enableIcon();
			}
			else
			{
				this->getMesh()->getQuad()->setIndex(this->currentExplosionTextureIndex);
				this->currentExplosionTextureIndex++;
			}
		}
	}
}

bool HoshoyoExplosionSkill::cancelIfPossible()
{
	if (this->isActive() && this->status == HoshoyoExplosionSkillStatus::AIM)
	{
		Game::cursor->showCursor();
		this->active = false;
		return true;
	}
	return false;
}

void HoshoyoExplosionSkill::execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId)
{
	this->status = HoshoyoExplosionSkillStatus::EXECUTION;
	explosionPosition = skillTargetPosition;
	this->getTransform()->translate(skillTargetPosition.x, skillTargetPosition.y, 0.1f);
	this->currentExplosionTextureIndex = 0;
	this->skillIcon->disableIcon();
	this->active = true;
}

const float skillRadius = 10.0f;
const int skillDamage = 10;

void HoshoyoExplosionSkill::hitEnemiesOnSkillRadius()
{
	if (this->owner == PLAYER)
	{
		for (Monster* monster : *(this->monsters))
		{
			glm::vec3 diffVector = monster->getTransform()->getPosition() - explosionPosition;
			if (glm::length(diffVector) < skillRadius && monster->isAlive())
				monster->doDamage(skillDamage);
		}
	}
}