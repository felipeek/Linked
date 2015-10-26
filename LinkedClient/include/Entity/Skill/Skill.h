#include "Entity.h"
#include "Movement.h"
#include "TextRenderer.h"

class Monster;
class Text;
class SkillIcon;

#pragma once

enum SkillSlot{
	SLOT_1 = 1,
	SLOT_2 = 2,
	SLOT_3 = 3,
	SLOT_4 = 4
};

class Skill : public Entity
{
public:
	Skill(std::vector<Monster*> *monsters);
	virtual ~Skill();

	virtual void prepareExecution(MovementDirection skillDirection) = 0;
	virtual void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId) = 0;
	virtual bool cancelIfPossible() = 0;
	virtual void update() = 0;
	virtual void render(Shader* primitiveShader, TextRenderer* textRenderer) = 0;

	SkillSlot getSlot();
	void setSlot(SkillSlot slot);
	bool isActive();

	// Creature that "owns" this skill and will use it eventually
	void setEntity(Entity* entity);
	Entity* getEntity();
	SkillIcon* getSkillIcon();
protected:
	SkillSlot slot;
	bool active;
	Entity* entity;
	SkillIcon* skillIcon;
	std::vector<Monster*> *monsters;
};

