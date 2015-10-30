#include "Movement.h"
#include "WorldObject.h"
#include <vector>
#define SKILL_COOLDOWN_DEFAULT 2

class Monster;
class Player;

#pragma once

enum SkillOwner {
	PLAYER,
	MONSTER
};

enum SkillSlot {
	SLOT_1 = 1,
	SLOT_2 = 2,
	SLOT_3 = 3,
	SLOT_4 = 4
};

class Skill : public WorldObject
{
public:
	Skill(SkillOwner owner, std::vector<Monster*> *monsters, std::vector<Player*>* players);
	~Skill();
	SkillSlot getSlot();
	void setSlot(SkillSlot slot);
	bool isActive();
	virtual void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId) = 0;
	virtual void update() = 0;

	// Creature that "owns" this skill and will use it eventually
	void setWorldObject(WorldObject* entity);
	WorldObject* getWorldObject();
	void setSkillOwner(SkillOwner owner);
	SkillOwner getSkillOwner() const;

	// Cooldown related
	int getCooldown() const;
	bool isOnCooldown() const;
protected:
	int cooldown = SKILL_COOLDOWN_DEFAULT;
	double cooldownContage = 0;
	SkillSlot slot;
	SkillOwner owner;
	bool active;
	WorldObject* worldObject;
	std::vector<Monster*> *monsters;

	void startCooldownContage();
	void resetCooldownContageForcibly();
};

