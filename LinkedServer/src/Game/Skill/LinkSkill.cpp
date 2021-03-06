#include "LinkSkill.h"
#include "Player.h"
#include "PacketController.h"
#include "Game.h"

#define LINK_SKILL_THRESHOLD 1.5f

LinkSkill::LinkSkill(SkillOwner owner) : Skill(owner)
{
	this->linked = false;
}

LinkSkill::~LinkSkill()
{
}

void LinkSkill::execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId)
{
	this->active = true;
	this->linked = true;

	Player* targetPlayer = PacketController::getPlayerOfClient(targetCreatureId);

	if (targetPlayer != nullptr && this->owner == PLAYER)
		((Player*)(this->worldObject))->setLink(targetPlayer);
}

void LinkSkill::update(std::vector<Monster*> *monsters, std::vector<Player*> *players)
{
	if (this->active == true) {
		this->active = false;
	}
}