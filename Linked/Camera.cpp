#include "Camera.h"
#include "Input.h"

Camera::Camera(glm::vec3 position, glm::vec3 orientation, float fov, float aspect, float zNear, float zFar)
{
	distance = 20.0f;
	angle = 2.0f;
	camPosition = position;
	camOrientation = orientation;
	upVector = glm::vec3(1, 0, 0);
	viewMatrix = glm::lookAt(position, orientation, upVector);

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
	viewMatrix = glm::lookAt(pos, camOrientation, glm::cross(upVector, camOrientation));
	updateViewProj();
}
void Camera::setCamOrientation(glm::vec3& ori)
{
	camOrientation = ori;
	viewMatrix = glm::lookAt(camPosition, ori, glm::cross(upVector, camOrientation));
	updateViewProj();
}

void Camera::setUpVector(glm::vec3& up)
{
	this->upVector = up;
	setCamOrientation(camOrientation);
}

void Camera::setProjectionMatrix(glm::mat4 mat)
{
	this->projectionMatrix = mat;
	updateViewProj();
}

void Camera::update(glm::vec3& playerPosition)
{
	glm::vec3 camPos = glm::vec3(playerPosition.x, playerPosition.y - (distance / angle), distance);
	glm::vec3 camOri = playerPosition;
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
	//if (Input::keyStates['-'])
	//{
	//	incDistance();
	//}
	//
	//if (Input::keyStates['='])
	//{
	//	decDistance();
	//}
	if (Input::wheel > 0)
	{
		incDistance();
		Input::wheel = 0;
	}
		
	if (Input::wheel < 0)
	{
		decDistance();
		Input::wheel = 0;
	}
		
}

glm::mat4& Camera::getView()
{
	return viewMatrix;
}
glm::mat4& Camera::getProjection()
{
	return projectionMatrix;
}
glm::vec3& Camera::getPosition()
{
	return camPosition;
}

glm::vec3& Camera::getUpVector()
{
	return upVector;
}