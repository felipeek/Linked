#include "Skill.h"

#pragma once
class ZurikiRageSkill : public Skill
{
public:
	ZurikiRageSkill(std::vector<Monster*>* monsters);
	~ZurikiRageSkill();
	void use(MovementDirection direction);
	void update();
	Text* getSkillText();
private:
	double usedTime = 0;
	void refreshTexture();
	void transformSkill();
	void attackMonsters();
	int lastTextureIndex;
	double textureChangeTime = 0;
	MovementDirection skillDirection;
	int supplementaryAngle = 0;
};