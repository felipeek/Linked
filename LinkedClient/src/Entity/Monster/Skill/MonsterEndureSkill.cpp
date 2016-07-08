#include "MonsterEndureSkill.h"
#include "Primitive.h"
#include "Input.h"
#include "SkillIcon.h"
#include "Monster.h"
#include "Game.h"
#include "Cursor.h"
#include "PacketController.h"
#include "TextRenderer.h"
#include "Audio.h"
#include "Player.h"
#include "LinkedTime.h"

MonsterEndureSkill::MonsterEndureSkill(SkillOwner owner) : Skill(owner)
{
	/* EXPLOSION ENTITY (THIS) */
	Mesh* hoshoyoEndureMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f, 4, 0));
	Transform* hoshoyoEndureTransform = new Transform(glm::vec3(520, 500, 1.5f), 0, glm::vec3(1, 0, 0), glm::vec3(7, 7, 7));
	Texture* hoshoyoEndureTexture = new Texture("./res/Skills/aura.png");
	this->setMesh(hoshoyoEndureMesh);
	this->setTransform(hoshoyoEndureTransform);
	this->setTexture(hoshoyoEndureTexture);

	/* SKILL ICON */
	this->skillIcon = nullptr;
}

MonsterEndureSkill::~MonsterEndureSkill()
{
}

void MonsterEndureSkill::render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer)
{
	if (this->active)
		Entity::render(primitiveShader);
}

void MonsterEndureSkill::prepareExecution(MovementDirection skillDirection)
{

}

void MonsterEndureSkill::update(std::vector<Monster*> *monsters, std::vector<Player*> *players, Player* localPlayer)
{
	if (this->isActive() && this->owner == SkillOwner::MONSTER)
	{
		double now = LinkedTime::getTime();
		Monster* owner = (Monster*)this->getEntity();

		if (!owner->isAlive())
		{
			this->active = false;
		}
		else if (this->currentAuraTextureIndex == 16)
		{
			this->active = false;
			/*if (!Game::multiplayer)
			{*/
				owner->setKnockbackable(true);
				owner->setTotalSpeed(owner->getTotalSpeed() - SPEED_BONUS);
			//}
		}
		else
		{
			glm::vec3 auraNewPos = owner->getTransform()->getPosition();
			this->getTransform()->translate(auraNewPos.x, auraNewPos.y, 0.01f);

			if (now > (this->timeReference + TIME_OFFSET))
			{
				this->getMesh()->getQuad()->setIndex(this->currentAuraTextureIndex);
				this->currentAuraTextureIndex++;
				this->timeReference = now;
			}
		}
	}
}

bool MonsterEndureSkill::cancelIfPossible()
{
	return false;
}

void MonsterEndureSkill::execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId)
{
	if (this->owner == SkillOwner::MONSTER)
	{
		this->active = true;
		Monster* owner = (Monster*)this->getEntity();
		glm::vec3 initialAuraPos = owner->getTransform()->getPosition();
		this->getTransform()->translate(initialAuraPos.x, initialAuraPos.y, 0.01f);
		this->currentAuraTextureIndex = 0;
		this->timeReference = LinkedTime::getTime();
	/*	if (!Game::multiplayer)
		{*/
			owner->setKnockbackable(false);
			owner->setTotalSpeed(owner->getTotalSpeed() + SPEED_BONUS);
		//}
	}
}