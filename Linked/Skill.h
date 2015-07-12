#pragma once

enum SkillSlot{
	SLOT_1,
	SLOT_2,
	SLOT_3,
	SLOT_4
};

class Skill
{
public:
	Skill();
	~Skill();
	SkillSlot getSlot();
};

