#include "Light.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Light::Light(glm::vec3 position, glm::vec3 color)
{
	lightPosition = position;
	lightColor = color;
	day = true;
	dayDelta = 60.0f;
	timeRate = 0.0f;
}


Light::~Light()
{
}

void Light::input()
{
	if (Input::keyStates[1 + 320])
		dayDelta += 0.5f;
	if (Input::keyStates[9 + 320])
		dayDelta -= 0.5f;
}

void Light::update(glm::vec3& playerPosition)
{
	glm::vec3 nightSuppressor = glm::vec3(0.83f - 0.5f, 0.85f - 0.5f, 0.85f - 0.5f);
	
	glm::mat4 rot = glm::rotate(glm::mat4(), dayDelta, glm::cross(glm::vec3(-2, -1, 0), glm::vec3(0, 0, 1)));

	glm::vec4 initialPosition = glm::vec4(-80.0f, -40.0f, 0, 1.0f);
	initialPosition = rot * initialPosition;

	glm::vec3 endPos = glm::vec3(initialPosition);

	lightPosition = playerPosition + endPos;

	if (lightPosition.z < 10.0f && lightPosition.z > 0)
	{
		float intensity = lightPosition.z / 10.0f;
		if (day)
			lightColor = glm::vec3(intensity, intensity-0.07f, intensity-0.13f);
		else
		{
			lightColor = glm::vec3(intensity-0.07, intensity - 0.05f, intensity - 0.05f) * nightSuppressor;
		}
			
	}
	if (lightPosition.z < 0)
	{
		dayDelta += 180.0f;
		day = !day;
	}

	dayDelta += timeRate;

}

void Light::setPosition(glm::vec3 position)
{
	this->lightPosition = position;
}