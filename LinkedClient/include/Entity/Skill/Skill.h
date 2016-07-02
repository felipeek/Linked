#pragma once
#include "Entity.h"
#include "Movement.h"

#define SKILL_COOLDOWN_DEFAULT 2

class Monster;
class SkillIcon;
class Player;
class TextRenderer;

enum SkillOwner
{
	PLAYER,
	MONSTER
};

enum SkillSlot{
	SLOT_1 = 1,
	SLOT_2 = 2,
	SLOT_3 = 3,
	SLOT_4 = 4
};

class Skill : public Entity
{
public:
	Skill(SkillOwner owner);
	virtual ~Skill();

	virtual void prepareExecution(MovementDirection skillDirection) = 0;
	virtual void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId) = 0;
	virtual bool cancelIfPossible() = 0;
	virtual void update(std::vector<Monster*> *monsters, std::vector<Player*> *players, Player* localPlayer) = 0;
	virtual void render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer) = 0;

	SkillSlot getSlot();
	void setSlot(SkillSlot slot);
	bool isActive();

	// Creature that "owns" this skill and will use it eventually
	void setEntity(Entity* entity);
	Entity* getEntity();
	void setSkillOwner(SkillOwner owner);
	SkillOwner getSkillOwner() const;

	SkillIcon* getSkillIcon();

	// Cooldown related
	int getCooldown() const;
	bool isOnCooldown() const;
protected:
	int cooldown = SKILL_COOLDOWN_DEFAULT;
	double cooldownContage = 0;
	SkillSlot slot;
	SkillOwner owner;
	bool active;
	Entity* entity;
	SkillIcon* skillIcon;

	void startCooldownContage();
	void resetCooldownContageForcibly();
};

