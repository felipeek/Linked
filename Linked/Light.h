#pragma once
#include <glm\glm.hpp>

class Light
{
public:
	Light(glm::vec3 position, glm::vec3 color);
	~Light();
	glm::vec3 lightPosition;
	glm::vec3 lightColor;
};

