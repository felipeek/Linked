#include "Entity.h"
#include "Movement.h"

class Monster;
class Text;
class SkillIcon;

#pragma once

enum SkillSlot{
	SLOT_1,
	SLOT_2,
	SLOT_3,
	SLOT_4
};

class Skill : public Entity
{
public:
	Skill(std::vector<Monster*> *monsters);
	~Skill();
	virtual void render(Shader* primitiveShader, Shader* fontShader) = 0;
	SkillSlot getSlot();
	void setSlot(SkillSlot slot);
	bool isActive();
	virtual void use(MovementDirection direction) = 0;
	virtual bool cancelIfPossible() = 0;
	virtual void update() = 0;

	// Creature that "owns" this skill and will use it eventually
	void setEntity(Entity* entity);
	Entity* getEntity();
	Text* getSkillText();
	SkillIcon* getSkillIcon();
protected:
	SkillSlot slot;
	bool active;
	Entity* entity;
	SkillIcon* skillIcon;
	std::vector<Monster*> *monsters;
	Text* skillText;
};

