#include "Cursor.h"
#include "Input.h"
#include "Primitive.h"

Cursor::Cursor() : Entity(nullptr, nullptr, nullptr)
{
	// Cursor
	this->transform = new Transform(glm::vec3(0, 0, 0), glm::vec3(0.3f, 0.3f, 0.3f));
	this->mesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 0.2f, 0.2f));
	this->texture = new Texture("./res/Textures/target.png");

	this->cursorRot = 0;
}


Cursor::~Cursor()
{
}

void Cursor::update()
{
	this->getTransform()->translate(Input::mouseAttack.getOrthoCoords().x, Input::mouseAttack.getOrthoCoords().y, 0);
	this->getTransform()->rotate(cursorRot, glm::vec3(0, 0, 1));
	cursorRot += CURSOR_ROTATION_SPEED;
}