#include "MonsterExplosionSkill.h"
#include "SkillIcon.h"
#include "Monster.h"
#include "Game.h"
#include "TextRenderer.h"
#include "AudioController.h"
#include "Player.h"
#include "PacketController.h"

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

	/* SKILL ICON */
	this->skillIcon = nullptr;
}

MonsterExplosionSkill::~MonsterExplosionSkill()
{
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
		if (this->currentExplosionTextureIndex == 0)
			this->hitPlayerIfOnRadius(localPlayer);
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
	AudioController::getHoshoyoExplosionSkillAudio().play();
}

void MonsterExplosionSkill::hitPlayerIfOnRadius(Player* localPlayer)
{
	if (this->owner == MONSTER)
	{
		Monster* owner = (Monster*)this->getEntity();
		int skillDamage = SKILL_DAMAGE_FACTOR * owner->getTotalAttack();
		glm::vec3 diffVector = localPlayer->getTransform()->getPosition() - explosionPosition;
		if (glm::length(diffVector) < MONSTER_EXPLOSION_SKILL_MAX_RADIUS && localPlayer->isAlive())
		{
			if (Game::multiplayer) PacketController::sendPlayerDamageToServer(skillDamage);
			localPlayer->doDamage(skillDamage);
		}
	}
}