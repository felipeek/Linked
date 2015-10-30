#include "MouseAttack.h"
#include "Input.h"
#include "LinkedTime.h"
#include "Camera.h"
#include "Display.h"
#include <iostream>


MouseAttack::MouseAttack()
{
	screenPos = glm::vec2(0, 0);
	lastTime = LinkedTime::getTime();
}

MouseAttack::~MouseAttack()
{
}

void MouseAttack::setAttackPos(float x, float y)
{
	screenPos.x = x;
	screenPos.y = y;
}

void MouseAttack::setMouseCoords(int mouseX, int mouseY)
{
	float x = (2.0f * mouseX) / WWID - 1.0f;
	float y = 1.0f - (2.0f * mouseY) / WHEI;
	float z = 1.0f;
	//std::cout << x << " " << y << std::endl;
	glm::vec3 screenPosition = glm::vec3(x, y,z);

	glm::vec4 ray_clip = glm::vec4(screenPosition.x, screenPosition.y, -1.0, 1.0);
	glm::vec4 ray_eye = glm::inverse(camera->getProjection()) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);
	glm::vec3 ray_world = glm::vec3(glm::inverse(camera->getView()) * ray_eye);
	ray_world = glm::normalize(ray_world);
	this->rayWorld = ray_world;
	calculateIntersection(ray_world, camera->getPosition());
	//std::cout << ray_world.x << " " << ray_world.y << " " << ray_world.z << std::endl;
}

glm::vec2 MouseAttack::getScreenPos()
{
	return this->screenPos;
}

glm::vec3 MouseAttack::attack()
{
	glm::vec3 attackVector = (glm::vec3(screenPos, 1.0f) * glm::vec3(VECTORSIZE, VECTORSIZE, 1));
	return attackVector;
}


void MouseAttack::setCamera(Camera* camera)
{
	this->camera = camera;
}

void MouseAttack::update()
{
	this->calculateIntersection(rayWorld, camera->getPosition());
}

void MouseAttack::calculateIntersection(glm::vec3& worldRay, glm::vec3& cameraPosition)
{
	float t = -(cameraPosition.z) / worldRay.z;

	float x_I = cameraPosition.x + (t * worldRay.x);
	float y_I = cameraPosition.y + (t * worldRay.y);
	float z_I = cameraPosition.z + (t * worldRay.z);

	mouseIntersection = glm::vec3(x_I, y_I, z_I);
}

glm::vec3& MouseAttack::getMouseIntersection()
{
	return mouseIntersection;
}