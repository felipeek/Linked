#include "Skill.h"

enum SkillStatus
{
	AIM,
	EXECUTION
};

#pragma once
class HoshoyoExplosionSkill : public Skill
{
public:
	HoshoyoExplosionSkill(std::vector<Monster*>* monsters);
	~HoshoyoExplosionSkill();

	void render(Shader* primitiveShader, TextRenderer* textRenderer);
	void prepareExecution(MovementDirection skillDirection);
	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	bool cancelIfPossible();
	void update();
private:
	void hitEnemiesOnSkillRadius();
	
	Entity* aimEntity;
	SkillStatus status;
	glm::vec3 explosionPosition;

	int currentExplosionTextureIndex;
};

