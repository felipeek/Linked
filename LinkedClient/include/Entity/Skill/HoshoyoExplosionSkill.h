#pragma once
#include "Skill.h"

#define HOSHOYO_EXPLOSION_SKILL_ICON_ENABLED "./res/Skills/hoshoyo_explosion_enabled.png"
#define HOSHOYO_EXPLOSION_SKILL_ICON_DISABLED "./res/Skills/hoshoyo_explosion_disabled.png"
#define HOSHOYO_EXPLOSION_SKILL_MAX_RADIUS 25

class Audio;

enum class HoshoyoExplosionSkillStatus
{
	AIM,
	EXECUTION
};

#pragma once
class HoshoyoExplosionSkill : public Skill
{
public:
	HoshoyoExplosionSkill(SkillOwner owner);
	~HoshoyoExplosionSkill();

	void render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer);
	void prepareExecution(MovementDirection skillDirection);
	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	bool cancelIfPossible();
	void update(std::vector<Monster*> *monsters, std::vector<Player*> *players, Player* localPlayer);
private:
	void hitEnemiesOnSkillRadius(std::vector<Monster*> *monsters);
	void sendExecutionToServer();

	Entity* aimEntity;
	Entity* rangeEntity;
	HoshoyoExplosionSkillStatus status;
	glm::vec3 explosionPosition;

	int currentExplosionTextureIndex;
	float cursorRot;

	Audio* skillAudio;
};

