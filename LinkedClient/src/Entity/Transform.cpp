#include "Transform.h"

Transform::Transform() : Transform(glm::vec3(0,0,0)){}

Transform::Transform(glm::vec3 position)
{
	translationMatrix = glm::translate(glm::mat4(), position);
	mPosition = position;
	scaleMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::mat4(1.0f);
	angleY = 0;
	updateModel();
}

Transform::Transform(glm::vec3 position, float angle, glm::vec3 axis, glm::vec3 size) : Transform(position)
{
	rotate(angle, axis);
	scale(size.x, size.y, size.z);
	updateModel();
}

Transform::Transform(glm::vec3 position, glm::vec3 size) : Transform(position)
{
	scale(size.x, size.y, size.z);
	updateModel();
}

Transform::~Transform()
{
}

float Transform::getAngleY()
{
	return angleY;
}

void Transform::translate(float x, float y, float z)
{
	mPosition = glm::vec3(x, y, z);
	translationMatrix = glm::translate(glm::mat4(), mPosition);
	updateModel();
}
void Transform::rotate(float angle, glm::vec3 axe)
{
	rotationMatrix = glm::rotate(glm::mat4(), angle, axe);
	updateModel();
}
void Transform::scale(float x, float y, float z)
{
	mScale = glm::vec3(x, y, z);
	scaleMatrix = glm::scale(glm::mat4(), mScale);
	updateModel();
}

void Transform::incTranslate(float x, float y, float z)
{
	mPosition.x += x;
	mPosition.y += y;
	mPosition.z += z;

	translationMatrix = glm::translate(glm::mat4(), mPosition);

	updateModel();
}

void Transform::incRotateY(float amt)
{
	angleY += amt;
	rotationMatrix = rotationMatrix * glm::rotate(glm::mat4(), angleY, glm::vec3(0, 1, 0));
	updateModel();
}

void Transform::incScale(float x, float y, float z)
{
	mScale.x += x;
	mScale.y += y;
	mScale.z += z;

	scaleMatrix = glm::scale(glm::mat4(), mScale);

	updateModel();
}

void Transform::updateModel()
{
	model = translationMatrix * rotationMatrix * scaleMatrix;
}

glm::vec3 Transform::getPosition() const
{
	return mPosition;
}

glm::vec3 Transform::getScale()
{
	return mScale;
}

void Transform::setPosition(glm::vec3& pos)
{
	this->mPosition = pos;
}