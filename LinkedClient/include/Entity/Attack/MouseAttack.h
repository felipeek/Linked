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
	glm::vec2 getScreenPos();
	glm::vec2 getOrthoCoords();
private:
	Camera* camera;
	
	glm::vec3 mouseIntersection;
	double lastTime;
	

	void calculateIntersection(glm::vec3& worldRay, glm::vec3& cameraPosition);
	glm::vec3 rayWorld;
	glm::vec2 screenPos;
};

