#pragma once
#include <glm\glm.hpp>

#define VECTORSIZE 10
#define COOLDOWN 0

class Camera;

class MouseAttack
{
public:
	MouseAttack(Camera* camera);
	MouseAttack();
	~MouseAttack();
	
	glm::vec3 attack();
	void setAttackPos(float x, float y);
	void setMouseCoords(int x, int y);

	glm::vec3& getMouseIntersection();

	void setCamera(Camera* camera);
	void update();
private:
	Camera* camera;
	glm::vec2 screenPos;
	glm::vec3 mouseIntersection;
	double lastTime;
	glm::vec3 rayWorld;

	void calculateIntersection(glm::vec3& worldRay, glm::vec3& cameraPosition);
	
};

