#include "HoshoyoExplosionSkill.h"
#include "Primitive.h"
#include "Input.h"
#include "SkillIcon.h"
#include "Monster.h"

HoshoyoExplosionSkill::HoshoyoExplosionSkill(std::vector<Monster*>* monsters) : Skill(monsters)
{
	/* AIM ENTITY */
	Mesh* aimMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f, 1, 12));
	Transform* aimTransform = new Transform(glm::vec3(520, 500, 0.1f), 0, glm::vec3(1, 0, 0), glm::vec3(10, 10, 10));
	Texture* aimTexture = new Texture("./res/Skills/aim.png");
	this->aimEntity = new Entity(aimTransform, aimMesh, aimTexture);

	/* EXPLOSION ENTITY (THIS) */
	Mesh* hoshoyoExplosionMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f, 9, 12));
	Transform* hoshoyoExplosionTransform = new Transform(glm::vec3(520, 500, 1.5f), 0, glm::vec3(1, 0, 0), glm::vec3(10, 10, 10));
	Texture* hoshoyoExplosionTexture = new Texture("./res/Skills/hoshoyoexplosion.png");
	this->setMesh(hoshoyoExplosionMesh);
	this->setTransform(hoshoyoExplosionTransform);
	this->setTexture(hoshoyoExplosionTexture);

	/* SKILL ICON */
	Texture* enabledSkillIconTexture = new Texture("./res/Skills/hoshoyoexplosion_icon.png");
	Texture* disabledSkillIconTexture = new Texture("./res/Skills/hoshoyoexplosion_icon_black.png");
	this->skillIcon = new SkillIcon(enabledSkillIconTexture, disabledSkillIconTexture, SLOT_1);
}

HoshoyoExplosionSkill::~HoshoyoExplosionSkill()
{
	if (this->aimEntity != NULL)
		delete this->aimEntity;
}

void HoshoyoExplosionSkill::render(Shader* primitiveShader, TextRenderer* textRenderer)
{
	if (this->status == AIM)
		this->aimEntity->render(primitiveShader);
	else if (this->status == EXECUTION)
	{
		Entity::render(primitiveShader);
		// temporary (just 4fun)
		textRenderer->renderText("Hoshoyo's Explosion!", 710, 525, 0.25f, glm::vec3(0.9f, 0.9f, 0.9f));
	}
}

void HoshoyoExplosionSkill::use(MovementDirection direction)
{
	if (!this->active)
	{
		this->active = true;
		this->status = AIM;
	}
}

void HoshoyoExplosionSkill::update()
{
	if (this->isActive())
	{
		if (this->status == AIM)
		{
			glm::vec3 mousePos = Input::mouseAttack.getMouseIntersection();
			this->aimEntity->getTransform()->translate(mousePos.x, mousePos.y, 0.1f);

			if (Input::attack)
				this->executeSkill(mousePos);
		}
		else if (this->status == EXECUTION)
		{
			if (this->currentExplosionTextureIndex % 27 == 0)
				this->hitEnemiesOnSkillRadius();
			if (this->currentExplosionTextureIndex == 81)
			{
				this->active = false;
				this->skillIcon->enableIcon();
			}
			else
			{
				this->getMesh()->getQuad()->setIndex(this->currentExplosionTextureIndex);
				this->currentExplosionTextureIndex++;
			}
		}
	}
}

bool HoshoyoExplosionSkill::cancelIfPossible()
{
	if (this->isActive() && this->status == AIM)
	{
		this->active = false;
		return true;
	}
	return false;
}

void HoshoyoExplosionSkill::executeSkill(glm::vec3 position)
{
	this->status = EXECUTION;
	explosionPosition = position;
	this->getTransform()->translate(position.x, position.y, 0.1f);
	this->currentExplosionTextureIndex = 0;
	this->skillIcon->disableIcon();
}

const float skillRadius = 10.0f;
const int skillDamage = 10;

void HoshoyoExplosionSkill::hitEnemiesOnSkillRadius()
{
	for (Monster* monster : *(this->monsters))
	{
		glm::vec3 diffVector = monster->getTransform()->getPosition() - explosionPosition;
		if (length(diffVector) < skillRadius)
			monster->doDamage(skillDamage);
	}
}