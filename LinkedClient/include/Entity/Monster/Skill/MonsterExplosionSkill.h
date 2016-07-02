#pragma once
#include "Skill.h"

#define MONSTER_EXPLOSION_SKILL_MAX_RADIUS 25

class Audio;

#pragma once
class MonsterExplosionSkill : public Skill
{
public:
	MonsterExplosionSkill(SkillOwner owner);
	~MonsterExplosionSkill();

	void render(Shader* primitiveShader, Shader* skillShader, TextRenderer* textRenderer);
	void prepareExecution(MovementDirection skillDirection);
	void execute(MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
	bool cancelIfPossible();
	void update(std::vector<Monster*> *monsters, std::vector<Player*> *players, Player* localPlayer);
private:
	void hitPlayerIfOnRadius(Player* localPlayer);
	glm::vec3 explosionPosition;
	int currentExplosionTextureIndex;
	Audio* skillAudio;
};

