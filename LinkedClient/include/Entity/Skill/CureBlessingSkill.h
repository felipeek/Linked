#pragma once
#include "Skill.h"

#define CURE_BLESSING_SKILL_COOLDOWN 15
#define CURE_BLESSING_SKILL_PERCENTAGE 30

enum class CureBlessingSkillStatus
{
	AIM,
	WAITING_FOR_SERVER_RESPONSE,
	IDLE,
	EXECUTION
};

class CureBlessingSkill : public Skill
{
public:
	CureBlessingSkill(SkillOwner owner, std::vector<Monster*>* monsters, std::vector<Player*>* players, Player** localPlayer);
	~CureBlessingSkill();
	void render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer);
	void prepareExecution(MovementDirection skillDirection);
	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	bool cancelIfPossible();
	void update();
private:
	Entity* aimEntity;
	CureBlessingSkillStatus status;
	Player* getTargetPlayer();
	bool checkCooldown = false;
};

