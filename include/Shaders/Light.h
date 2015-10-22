#pragma once
#include <glm\glm.hpp>

class Light
{
public:
	Light(glm::vec3 position, glm::vec3 color);
	~Light();

	void input();
	void update(glm::vec3& playerPosition);

	glm::vec3 lightPosition;
	glm::vec3 lightColor;

	void setPosition(glm::vec3 position);

	bool day;
	float dayDelta;
	float timeRate;
};

