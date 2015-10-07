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

	void render(Shader* primitiveShader, Shader* fontShader);
	void use(MovementDirection direction);
	bool cancelIfPossible();
	void update();
private:
	void executeSkill(glm::vec3 position);
	void hitEnemiesOnSkillRadius();
	
	Entity* aimEntity;
	SkillStatus status;
	glm::vec3 explosionPosition;

	int currentExplosionTextureIndex;
};

