#include "SkillIcon.h"
#include "Primitive.h"

SkillIcon::SkillIcon(Texture* enabledTexture, Texture* disabledTexture, SkillSlot slot) : Entity(NULL, NULL, texture)
{
	Transform* skillIconTransform = new Transform(glm::vec3(-0.63f, -0.933f, 0), glm::vec3(0.6f*0.03f, 0.03f, 0.03f));
	Mesh* skillIconMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f));

	this->setTransform(skillIconTransform);
	this->setMesh(skillIconMesh);
	this->setTexture(enabledTexture);
	this->enabledTexture = enabledTexture;
	this->disabledTexture = disabledTexture;
	this->setSlot(slot);
	this->enableIcon();
}


SkillIcon::~SkillIcon()
{
	if (mesh != nullptr)
		delete mesh;
	if (enabledTexture != nullptr)
		delete enabledTexture;
	if (disabledTexture != nullptr)
		delete disabledTexture;
}

void SkillIcon::enableIcon()
{
	if (!this->enabled)
	{
		this->setTexture(enabledTexture);
		this->enabled = true;
	}
}

void SkillIcon::disableIcon()
{
	if (this->enabled)
	{
		this->setTexture(disabledTexture);
		this->enabled = false;
	}
}

void SkillIcon::setIconCooldown(int cd)
{
	// TO DO
}

bool SkillIcon::isEnabled()
{
	return this->enabled;
}

void SkillIcon::setSlot(SkillSlot slot)
{
	switch (slot)
	{
		case SLOT_1:
			this->getTransform()->translate(SKILL_SLOT1_OFFSET_X, SKILL_SLOT_OFFSET_Y, 0);
			this->getTransform()->scale(SKILL_SLOT_SIZE);
			break;
		case SLOT_2:
			this->getTransform()->translate(SKILL_SLOT2_OFFSET_X, SKILL_SLOT_OFFSET_Y, 0);
			this->getTransform()->scale(SKILL_SLOT_SIZE);
			break;
		case SLOT_3:
			this->getTransform()->translate(SKILL_SLOT3_OFFSET_X, SKILL_SLOT_OFFSET_Y, 0);
			this->getTransform()->scale(SKILL_SLOT_SIZE);
			break;
		case SLOT_4:
			this->getTransform()->translate(SKILL_SLOT4_OFFSET_X, SKILL_SLOT_OFFSET_Y, 0);
			this->getTransform()->scale(SKILL_SLOT_SIZE);
			break;
	}
	
	this->slot = slot;
}

SkillSlot SkillIcon::getSlot()
{
	return this->slot;
}