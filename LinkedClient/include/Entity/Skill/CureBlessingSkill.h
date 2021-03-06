#pragma once
#include "Skill.h"

#define CURE_BLESSING_SKILL_ICON_ENABLED "./res/Skills/cure_blessing_enabled.png"
#define CURE_BLESSING_SKILL_ICON_DISABLED "./res/Skills/cure_blessing_disabled.png"
#define CURE_BLESSING_SKILL_COOLDOWN 15
#define CURE_BLESSING_SKILL_PERCENTAGE 30
#define CURE_BLESSING_SKILL_AIM_THRESHOLD 2.0f

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
	CureBlessingSkill(SkillOwner owner);
	~CureBlessingSkill();
	void render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer);
	void prepareExecution(MovementDirection skillDirection);
	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	bool cancelIfPossible();
	void update(std::vector<Monster*> *monsters, std::vector<Player*> *players, Player* localPlayer);
private:
	Entity* aimEntity;
	CureBlessingSkillStatus status;
	Player* getTargetPlayer(std::vector<Player*> *players);
	bool checkCooldown = false;
	float cursorRot;
	void sendExecutionToServer(glm::vec3 mousePos, Player* targetPlayer);
};

