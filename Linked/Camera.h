#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#define ZOOMSPEED 2.5f
#define MINDISTANCE 10
#define MAXDISTANCE 50

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 orientation, float fov, float aspect, float zNear, float zFar);
	~Camera();

	void setCamPosition(glm::vec3& pos);
	void setCamOrientation(glm::vec3& ori);
	void setUpVector(glm::vec3& up);

	void update(glm::vec3& playerPos);
	void input();
	void incDistance();
	void decDistance();

	glm::mat4 viewProj;

	glm::mat4& getView();
	glm::mat4& getProjection();
	glm::vec3& getPosition();
	glm::vec3& getUpVector();

	void setProjectionMatrix(glm::mat4 mat);
	
private:
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	glm::vec3 camPosition;
	glm::vec3 camOrientation;
	glm::vec3 upVector;

	float distance;
	float angle;

	void updateViewProj();
};

