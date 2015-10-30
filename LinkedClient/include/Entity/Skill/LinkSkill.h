#pragma once
#include "Skill.h"

enum class LinkSkillStatus
{
	AIM,
	EXECUTION
};

class LinkSkill : public Skill
{
public:
	LinkSkill(SkillOwner owner, std::vector<Monster*>* monsters, std::vector<Player*>* players, Player** localPlayer);
	~LinkSkill();
	void prepareExecution(MovementDirection skillDirection);
	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	bool cancelIfPossible();
	void update();
	void render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer);
private:
	Player* getTargetPlayer();
	Entity* aimEntity;
	LinkSkillStatus status;
	bool linked;
};

