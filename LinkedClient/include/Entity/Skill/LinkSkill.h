#pragma once
#include "Skill.h"

#define LINK_SKILL_ICON_ENABLED "./res/Skills/link_enabled.png"
#define LINK_SKILL_ICON_DISABLED "./res/Skills/link_disabled.png"
#define LINK_SKILL_AIM_THRESHOLD 2.0f

enum class LinkSkillStatus
{
	AIM,
	EXECUTION
};

class LinkSkill : public Skill
{
public:
	LinkSkill(SkillOwner owner);
	~LinkSkill();
	void prepareExecution(MovementDirection skillDirection);
	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	bool cancelIfPossible();
	void update(std::vector<Monster*> *monsters, std::vector<Player*> *players, Player* localPlayer);
	void render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer);
private:
	Player* getTargetPlayer(std::vector<Player*> *players);
	Entity* aimEntity;
	LinkSkillStatus status;
	float cursorRot;
	void sendExecutionToServer(glm::vec3 mousePos, Player* targetPlayer);
};

