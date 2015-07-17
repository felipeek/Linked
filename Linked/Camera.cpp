#include "Camera.h"
#include "Input.h"

Camera::Camera(glm::vec3 position, glm::vec3 orientation, float fov, float aspect, float zNear, float zFar)
{
	distance = 20.0f;
	angle = 2;
	camPosition = position;
	camOrientation = orientation;
	viewMatrix = glm::lookAt(position, orientation, glm::vec3(0, 1, 0));
	projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
	updateViewProj();
}


Camera::~Camera()
{
}

void Camera::updateViewProj()
{
	viewProj = projectionMatrix * viewMatrix;
}

void Camera::setCamPosition(glm::vec3& pos)
{
	camPosition = pos;
	viewMatrix = glm::lookAt(pos, camOrientation, glm::vec3(0, 1, 0));
	updateViewProj();
}
void Camera::setCamOrientation(glm::vec3& ori)
{
	camOrientation = ori;
	viewMatrix = glm::lookAt(camPosition, ori, glm::vec3(0, 1, 0));
	updateViewProj();
}

void Camera::update(glm::vec3& playerPosition)
{
	glm::vec3 camOri = glm::vec3(playerPosition.x , playerPosition.y, 0);
	glm::vec3 camPos = glm::vec3(playerPosition.x, playerPosition.y - (distance/angle), distance);

	setCamPosition(camPos);
	setCamOrientation(camOri);
}

void Camera::incDistance()
{
	if (distance <= MAXDISTANCE)
		distance += ZOOMSPEED;
}
void Camera::decDistance()
{
	if (distance >= MINDISTANCE)
		distance -= ZOOMSPEED;
}

void Camera::input()
{
	if (Input::keyStates['-'])
	{
		incDistance();
	}

	if (Input::keyStates['='])
	{
		decDistance();
	}
}