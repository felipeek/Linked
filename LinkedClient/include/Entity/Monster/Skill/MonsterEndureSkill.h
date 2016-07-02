#pragma once
#include "Skill.h"

#define TIME_OFFSET 0.2f
#define SPEED_BONUS 10

class MonsterEndureSkill : public Skill
{
public:
	MonsterEndureSkill(SkillOwner owner);
	~MonsterEndureSkill();

	void render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer);
	void prepareExecution(MovementDirection skillDirection);
	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	bool cancelIfPossible();
	void update(std::vector<Monster*> *monsters, std::vector<Player*> *players, Player* localPlayer);
private:
	int currentAuraTextureIndex;
	double timeReference = 0;
};

