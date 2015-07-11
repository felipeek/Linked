#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 orientation, float fov, float aspect, float zNear, float zFar);
	~Camera();

	glm::mat4 viewProj;
private:
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	glm::vec3 camPosition;
	glm::vec3 camOrientation;

	void updateViewProj();
};

