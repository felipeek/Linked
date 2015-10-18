#include "Entity.h"
#include "Skill.h"

#define SKILL_SLOT_SIZE 0.6f*0.03f, 0.03f, 0.03f
#define SKILL_SLOT_OFFSET_Y -0.933f

#define SKILL_SLOT1_OFFSET_X -0.63f
#define SKILL_SLOT2_OFFSET_X -0.585f
#define SKILL_SLOT3_OFFSET_X -0.54f
#define SKILL_SLOT4_OFFSET_X -0.495f

#define WIDTHFACTOR 0.6f

#pragma once
class SkillIcon : public Entity
{
public:
	SkillIcon(Texture* enabledTexture, Texture* disabledTexture, SkillSlot slot);
	~SkillIcon();

	bool isEnabled();
	void enableIcon();
	void disableIcon();
	void setIconCooldown(int cd);
	void setSlot(SkillSlot slot);
	SkillSlot getSlot();
private:
	bool enabled;
	Texture* enabledTexture;
	Texture* disabledTexture;
	SkillSlot slot;
};