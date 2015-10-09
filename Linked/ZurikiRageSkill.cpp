#include "ZurikiRageSkill.h"
#include "Time.h"
#include "Mesh.h"
#include "Primitive.h"
#include "Creature.h"
#include "Monster.h"
#include "SkillIcon.h"

#include "Text.h"

const int TEXTURE_SPRITE_1 = 12;
const int TEXTURE_SPRITE_2 = 13;
const int TEXTURE_SPRITE_3 = 14;
const int TEXTURE_SPRITE_4 = 15;
const int TEXTURE_SPRITE_5 = 8;
const int TEXTURE_SPRITE_6 = 9;
const int TEXTURE_SPRITE_7 = 10;
const int TEXTURE_SPRITE_8 = 11;
const int TEXTURE_SPRITE_9 = 4;
const int TEXTURE_SPRITE_10 = 5;
const int TEXTURE_SPRITE_11 = 6;
const int TEXTURE_SPRITE_12 = 7;

const float SKILL_RANGE_COLLISION_HEIGHT = 14.0f;
const float SKILL_RANGE_COLLISION_WIDTH = 0.5f;
const float SKILL_DAMAGE_PER_HIT = 5;

const double SKILL_DELAY = 1.0f;
const double SKILL_TEXTURE_CHANGE_DELAY = 0.02f;

const double SQRT2 = 1.414213562373095f;

ZurikiRageSkill::ZurikiRageSkill(std::vector<Monster*>* monsters) : Skill(monsters)
{
	Mesh* zurikiRageMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f, 4, 12));
	Transform* zurikiRageTransform = new Transform(glm::vec3(520, 500, 1.5f), 0, glm::vec3(1, 0, 0), glm::vec3(10, 10, 10));
	Texture* zurikiRageTexture = new Texture("./res/Skills/zurikirage2.png");

	this->setMesh(zurikiRageMesh);
	this->setTransform(zurikiRageTransform);
	this->setTexture(zurikiRageTexture);

	/* SKILL ICON */
	Texture* enabledSkillIconTexture = new Texture("./res/Skills/zurikiicon.png");
	Texture* disabledSkillIconTexture = new Texture("./res/Skills/zurikiicon_black.png");
	this->skillIcon = new SkillIcon(enabledSkillIconTexture, disabledSkillIconTexture, SLOT_1);
}


ZurikiRageSkill::~ZurikiRageSkill()
{

}

void ZurikiRageSkill::render(Shader* primitiveShader, TextRenderer* textRenderer)
{
	Entity::render(primitiveShader);
	// temporary (just 4fun)
	textRenderer->renderText("Zuriki's Rage!", 750, 525, 0.25f, glm::vec3(0.9f, 0.9f, 0.9f));
}

void ZurikiRageSkill::use(MovementDirection direction)
{
	if (!this->isActive())
	{
		double now = Time::getTime();
		this->textureChangeTime = now;
		this->active = true;
		this->usedTime = Time::getTime();
		this->lastTextureIndex = 12;
		this->skillDirection = direction;
		this->getSkillIcon()->disableIcon();

		// rotate the entity depending on the direction the player is looking at.
		// supplementaryAngle is needed so when the skill is over it can be added to the angle (so the angle will be 0º again)
		switch (direction)
		{
			case TOP: this->getTransform()->rotate(0, glm::vec3(0, 0, 1)); supplementaryAngle = 0; break;
			case TOP_RIGHT: this->getTransform()->rotate(315, glm::vec3(0, 0, 1)); supplementaryAngle = 45; break;
			case TOP_LEFT: this->getTransform()->rotate(45, glm::vec3(0, 0, 1)); supplementaryAngle = 315; break;
			case BOTTOM: this->getTransform()->rotate(180, glm::vec3(0, 0, 1)); supplementaryAngle = 180; break;
			case BOTTOM_RIGHT: this->getTransform()->rotate(225, glm::vec3(0, 0, 1)); supplementaryAngle = 135; break;
			case BOTTOM_LEFT: this->getTransform()->rotate(135, glm::vec3(0, 0, 1)); supplementaryAngle = 225; break;
			case LEFT: this->getTransform()->rotate(90, glm::vec3(0, 0, 1)); supplementaryAngle = 270; break;
			case RIGHT: this->getTransform()->rotate(270, glm::vec3(0, 0, 1)); supplementaryAngle = 90; break;
		}
	}
}

bool ZurikiRageSkill::cancelIfPossible()
{
	return false;
}

void ZurikiRageSkill::update()
{
	if (this->isActive())
	{
		this->transformSkill();		// transform the skill (change the position if the player has moved)
		this->refreshTexture();		// refresh the sprite texture
		this->attackMonsters();		// do damage to monsters that are inside the skill range

		double now = Time::getTime();
		if ((now - this->usedTime) > SKILL_DELAY)
		{
			this->active = false;
			this->getSkillIcon()->enableIcon();
			this->getTransform()->rotate(supplementaryAngle, glm::vec3(0, 0, 1));
		}
	}
}

void ZurikiRageSkill::attackMonsters()
{
	glm::vec3 playerPosition = this->entity->getTransform()->getPosition();

	for (Monster* monster : *monsters)
	{
		glm::vec3 monsterPosition = monster->getTransform()->getPosition();
		glm::vec3 differenceVector = monsterPosition - playerPosition;
		unsigned int monsterCollisionRange = monster->getTotalCollisionRange()/10.0f;

		bool hit = false;

		// this will test if the monster is being hit by the skill
		switch (this->skillDirection)
		{
			case TOP:
				hit = monsterPosition.y > playerPosition.y &&
					monsterPosition.y < playerPosition.y + SKILL_RANGE_COLLISION_HEIGHT + monsterCollisionRange &&
					monsterPosition.x > playerPosition.x - SKILL_RANGE_COLLISION_WIDTH - monsterCollisionRange &&
					monsterPosition.x < playerPosition.x + SKILL_RANGE_COLLISION_WIDTH + monsterCollisionRange; break;
			case TOP_RIGHT:
				hit = monsterPosition.y > playerPosition.y - monsterCollisionRange &&
					monsterPosition.y < playerPosition.y + SKILL_RANGE_COLLISION_HEIGHT / SQRT2 + monsterCollisionRange &&
					monsterPosition.x > playerPosition.x - monsterCollisionRange &&
					monsterPosition.x < playerPosition.x + SKILL_RANGE_COLLISION_HEIGHT / SQRT2 + monsterCollisionRange &&
					abs((differenceVector.x - differenceVector.y) / SQRT2) < SKILL_RANGE_COLLISION_WIDTH + monsterCollisionRange; break;
			case TOP_LEFT:
				hit = monsterPosition.y > playerPosition.y - monsterCollisionRange &&
					monsterPosition.y < playerPosition.y + SKILL_RANGE_COLLISION_HEIGHT / SQRT2 + monsterCollisionRange &&
					monsterPosition.x < playerPosition.x + monsterCollisionRange &&
					monsterPosition.x > playerPosition.x - SKILL_RANGE_COLLISION_HEIGHT / SQRT2 - monsterCollisionRange &&
					abs((differenceVector.x + differenceVector.y) / SQRT2) < SKILL_RANGE_COLLISION_WIDTH + monsterCollisionRange; break;
			case BOTTOM:
				hit = monsterPosition.y < playerPosition.y + monsterCollisionRange &&
					monsterPosition.y > playerPosition.y - SKILL_RANGE_COLLISION_HEIGHT - monsterCollisionRange &&
					monsterPosition.x > playerPosition.x - SKILL_RANGE_COLLISION_WIDTH - monsterCollisionRange &&
					monsterPosition.x < playerPosition.x + SKILL_RANGE_COLLISION_WIDTH + monsterCollisionRange; break;
			case BOTTOM_RIGHT:
				hit = monsterPosition.y < playerPosition.y + monsterCollisionRange &&
					monsterPosition.y > playerPosition.y - SKILL_RANGE_COLLISION_HEIGHT / SQRT2 - monsterCollisionRange &&
					monsterPosition.x > playerPosition.x - monsterCollisionRange &&
					monsterPosition.x < playerPosition.x + SKILL_RANGE_COLLISION_HEIGHT / SQRT2 + monsterCollisionRange &&
					abs((differenceVector.x + differenceVector.y) / SQRT2) < SKILL_RANGE_COLLISION_WIDTH + monsterCollisionRange; break;
			case BOTTOM_LEFT:
				hit = monsterPosition.y < playerPosition.y + monsterCollisionRange &&
					monsterPosition.y > playerPosition.y - SKILL_RANGE_COLLISION_HEIGHT / SQRT2 - monsterCollisionRange &&
					monsterPosition.x < playerPosition.x + monsterCollisionRange &&
					monsterPosition.x > playerPosition.x - SKILL_RANGE_COLLISION_HEIGHT / SQRT2 - monsterCollisionRange &&
					abs((differenceVector.x - differenceVector.y) / SQRT2) < SKILL_RANGE_COLLISION_WIDTH + monsterCollisionRange; break;
			case LEFT:
				hit = monsterPosition.y > playerPosition.y - SKILL_RANGE_COLLISION_WIDTH - monsterCollisionRange &&
					monsterPosition.y < playerPosition.y + SKILL_RANGE_COLLISION_WIDTH + monsterCollisionRange &&
					monsterPosition.x < playerPosition.x + monsterCollisionRange &&
					monsterPosition.x > playerPosition.x - SKILL_RANGE_COLLISION_HEIGHT - monsterCollisionRange; break;
			case RIGHT:
				hit = monsterPosition.y > playerPosition.y - SKILL_RANGE_COLLISION_WIDTH - monsterCollisionRange &&
					monsterPosition.y < playerPosition.y + SKILL_RANGE_COLLISION_WIDTH + monsterCollisionRange &&
					monsterPosition.x > playerPosition.x - monsterCollisionRange &&
					monsterPosition.x < playerPosition.x + SKILL_RANGE_COLLISION_HEIGHT + monsterCollisionRange; break;
		}

		if (hit)
		{
			monster->doDamage(SKILL_DAMAGE_PER_HIT);
		}
	}

	
}

void ZurikiRageSkill::transformSkill()
{
	glm::vec3 entityPosition = this->getEntity()->getTransform()->getPosition();
	
	// transform the skill based on player position
	// obs.: some corrections are made so the texture will be placed correctly around the player

	switch (skillDirection)
	{
		case TOP: this->getTransform()->translate(entityPosition.x, entityPosition.y + 10, entityPosition.z); break;
		case TOP_RIGHT: this->getTransform()->translate(entityPosition.x + 7.5f, entityPosition.y + 7.5f, entityPosition.z); break;
		case TOP_LEFT: this->getTransform()->translate(entityPosition.x - 7.5f, entityPosition.y + 7.5f, entityPosition.z); break;
		case BOTTOM: this->getTransform()->translate(entityPosition.x, entityPosition.y - 12, entityPosition.z); break;
		case BOTTOM_RIGHT: this->getTransform()->translate(entityPosition.x + 8, entityPosition.y - 8, entityPosition.z); break;
		case BOTTOM_LEFT: this->getTransform()->translate(entityPosition.x - 8, entityPosition.y - 8, entityPosition.z); break;
		case LEFT: this->getTransform()->translate(entityPosition.x - 11, entityPosition.y, entityPosition.z); break;
		case RIGHT: this->getTransform()->translate(entityPosition.x + 11, entityPosition.y, entityPosition.z); break;
	}
}

void ZurikiRageSkill::refreshTexture()
{
	double now = Time::getTime();
	this->getMesh()->getQuad()->setIndex(lastTextureIndex);
	bool shouldChangeTexture = (now - textureChangeTime) > SKILL_TEXTURE_CHANGE_DELAY;

	if (shouldChangeTexture)
	{
		switch (lastTextureIndex)
		{
		case TEXTURE_SPRITE_1: lastTextureIndex = TEXTURE_SPRITE_2; break;
		case TEXTURE_SPRITE_2: lastTextureIndex = TEXTURE_SPRITE_3; break;
		case TEXTURE_SPRITE_3: lastTextureIndex = TEXTURE_SPRITE_4; break;
		case TEXTURE_SPRITE_4: lastTextureIndex = TEXTURE_SPRITE_5; break;
		case TEXTURE_SPRITE_5: lastTextureIndex = TEXTURE_SPRITE_6; break;
		case TEXTURE_SPRITE_6: lastTextureIndex = TEXTURE_SPRITE_7; break;
		case TEXTURE_SPRITE_7: lastTextureIndex = TEXTURE_SPRITE_8; break;
		case TEXTURE_SPRITE_8: lastTextureIndex = TEXTURE_SPRITE_9; break;
		case TEXTURE_SPRITE_9: lastTextureIndex = TEXTURE_SPRITE_10; break;
		case TEXTURE_SPRITE_10: lastTextureIndex = TEXTURE_SPRITE_11; break;
		case TEXTURE_SPRITE_11: lastTextureIndex = TEXTURE_SPRITE_12; break;
		case TEXTURE_SPRITE_12: lastTextureIndex = TEXTURE_SPRITE_9; break;
		default: lastTextureIndex = TEXTURE_SPRITE_9; break;
		}

		textureChangeTime = now;
	}
	
}