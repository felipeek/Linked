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
	LinkSkill(SkillOwner owner);
	~LinkSkill();
	void prepareExecution(MovementDirection skillDirection);
	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	bool cancelIfPossible();
	void update(std::vector<Monster*> *monsters, std::vector<Player*> *players, Player* localPlayer);
	void render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer);
private:
	Player* getTargetPlayer(Player* localPlayer, std::vector<Player*> *players);
	Entity* aimEntity;
	LinkSkillStatus status;
	bool linked;
	float cursorRot;
};

