#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 orientation, float fov, float aspect, float zNear, float zFar)
{
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