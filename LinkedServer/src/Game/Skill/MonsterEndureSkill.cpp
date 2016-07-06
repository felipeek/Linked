#include "MonsterEndureSkill.h"
#include "PacketController.h"
#include "Game.h"
#include "Player.h"

MonsterEndureSkill::MonsterEndureSkill(SkillOwner owner) : Skill(owner)
{
}

MonsterEndureSkill::~MonsterEndureSkill()
{
}

void MonsterEndureSkill::execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId)
{
}
void MonsterEndureSkill::update(std::vector<Monster*> *monsters, std::vector<Player*> *players)
{
}