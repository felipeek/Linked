#include "Cursor.h"
#include "Input.h"
#include "Primitive.h"
#include "Input.h"

Cursor::Cursor() : Entity(nullptr, nullptr, nullptr)
{
	// Cursor
	this->transform = new Transform(glm::vec3(0, 0, 0), glm::vec3(0.3f, 0.3f, 0.3f));
	this->mesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 0.2f, 0.2f));
	this->attackCursorTexture = new Texture("./res/Textures/target.png");
	this->normalCursorTexture = new Texture("./res/Textures/notarget.png");

	this->cursorRot = 0;
	this->mode = CursorMode::NORMAL;
	this->texture = normalCursorTexture;
	this->hide = false;
}


Cursor::~Cursor()
{
	this->texture = nullptr;
	delete normalCursorTexture;
	delete attackCursorTexture;
}

void Cursor::renderCursor(Shader* shader)
{
	if (!this->hide)
		Entity::render(shader);
}

void Cursor::update()
{
	if (Input::attack)
	{
		if (this->mode != CursorMode::ATTACK)
			this->setCursorMode(CursorMode::ATTACK);
	}
	else
	{
		if (this->mode != CursorMode::NORMAL)
			this->setCursorMode(CursorMode::NORMAL);
	}
	this->getTransform()->translate(Input::mouseAttack.getOrthoCoords().x, Input::mouseAttack.getOrthoCoords().y, 0);
	this->getTransform()->rotate(cursorRot, glm::vec3(0, 0, 1));
	if (this->mode == CursorMode::ATTACK)
		cursorRot += CURSOR_ROTATION_SPEED;
}

void Cursor::setCursorMode(CursorMode mode)
{
	switch (mode)
	{
		case CursorMode::ATTACK:
			this->texture = attackCursorTexture;
			break;
		case CursorMode::NORMAL:
			this->texture = normalCursorTexture;
			break;
	}
	this->mode = mode;
}

void Cursor::hideCursor()
{
	this->hide = true;
}

void Cursor::showCursor()
{
	this->hide = false;
}