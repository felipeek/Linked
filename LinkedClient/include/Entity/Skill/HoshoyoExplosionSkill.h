#pragma once
#include "Skill.h"

enum class HoshoyoExplosionSkillStatus
{
	AIM,
	EXECUTION
};

#pragma once
class HoshoyoExplosionSkill : public Skill
{
public:
	HoshoyoExplosionSkill(SkillOwner owner, std::vector<Monster*>* monsters, std::vector<Player*>* players, Player** localPlayer);
	~HoshoyoExplosionSkill();

	void render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer);
	void prepareExecution(MovementDirection skillDirection);
	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	bool cancelIfPossible();
	void update();
private:
	void hitEnemiesOnSkillRadius();
	
	Entity* aimEntity;
	HoshoyoExplosionSkillStatus status;
	glm::vec3 explosionPosition;

	int currentExplosionTextureIndex;
};

