#include "MonsterExplosionSkill.h"
#include "SkillIcon.h"
#include "Monster.h"
#include "Game.h"
#include "TextRenderer.h"
#include "Audio.h"
#include "Player.h"

// TODO: Skill Animation flow must be implemented using LinkedTime, not only the update call.

MonsterExplosionSkill::MonsterExplosionSkill(SkillOwner owner) : Skill(owner)
{
	/* EXPLOSION ENTITY (THIS) */
	Mesh* hoshoyoExplosionMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f, 4, 0));
	Transform* hoshoyoExplosionTransform = new Transform(glm::vec3(520, 500, 1.5f), 0, glm::vec3(1, 0, 0), glm::vec3(10, 10, 10));
	Texture* hoshoyoExplosionTexture = new Texture("./res/Skills/bomber_poring_explosion.png");
	this->setMesh(hoshoyoExplosionMesh);
	this->setTransform(hoshoyoExplosionTransform);
	this->setTexture(hoshoyoExplosionTexture);

	/* SKILL AUDIO */
	this->skillAudio = new Audio("./res/Audio/hoshoyo_explosion_skill.wav", AudioType::SOUND);

	/* SKILL ICON */
	this->skillIcon = nullptr;
}

MonsterExplosionSkill::~MonsterExplosionSkill()
{
	if (this->skillAudio != NULL)
		delete this->skillAudio;
}

void MonsterExplosionSkill::render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer)
{
	if (this->active)
		Entity::render(primitiveShader);
}

void MonsterExplosionSkill::prepareExecution(MovementDirection skillDirection)
{

}

void MonsterExplosionSkill::update(std::vector<Monster*> *monsters, std::vector<Player*> *players, Player* localPlayer)
{
	if (this->isActive())
	{
		if (!Game::multiplayer)
		{
			if (this->currentExplosionTextureIndex == 0)
				this->hitPlayerIfOnRadius(localPlayer);
		}
		if (this->currentExplosionTextureIndex == 12)
		{
			this->active = false;
			//this->skillIcon->enableIcon();
		}
		else
		{
			this->getMesh()->getQuad()->setIndex(this->currentExplosionTextureIndex);
			this->currentExplosionTextureIndex++;
		}
	}
}

bool MonsterExplosionSkill::cancelIfPossible()
{
	return false;
}

void MonsterExplosionSkill::execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId)
{
	this->active = true;
	explosionPosition = skillTargetPosition;
	this->getTransform()->translate(skillTargetPosition.x, skillTargetPosition.y, 0.1f);
	this->currentExplosionTextureIndex = 0;
	this->skillAudio->play();
}

const float skillRadius = 10.0f;
const int skillDamage = 10;

void MonsterExplosionSkill::hitPlayerIfOnRadius(Player* localPlayer)
{
	if (this->owner == MONSTER)
	{
		glm::vec3 diffVector = localPlayer->getTransform()->getPosition() - explosionPosition;
		if (glm::length(diffVector) < skillRadius && localPlayer->isAlive())
			localPlayer->doDamage(skillDamage);
	}
}