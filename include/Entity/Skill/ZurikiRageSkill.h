#include "Skill.h"

#pragma once
class ZurikiRageSkill : public Skill
{
public:
	ZurikiRageSkill(std::vector<Monster*>* monsters);
	~ZurikiRageSkill();
	void render(Shader* primitiveShader, TextRenderer* textRenderer);
	void use(MovementDirection direction);
	void update();
	bool cancelIfPossible();
private:
	double usedTime = 0;
	void refreshTexture();
	void transformSkill();
	void attackMonsters();
	int lastTextureIndex;
	double textureChangeTime = 0;
	MovementDirection skillDirection;
	float supplementaryAngle = 0;
};