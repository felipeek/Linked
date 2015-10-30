#pragma once
#include "Skill.h"

enum class SwapSkillStatus
{
	ANIMATION,
	EXECUTION
};

class SwapSkill : public Skill
{
public:
	SwapSkill(SkillOwner owner, std::vector<Monster*>* monsters, std::vector<Player*>* players, Player** localPlayer);
	~SwapSkill();
	void render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer);
	void prepareExecution(MovementDirection skillDirection);
	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	bool cancelIfPossible();
	void update();
private:
	SwapSkillStatus status;
	bool checkCooldown = false;
};