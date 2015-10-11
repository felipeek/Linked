#include "Light.h"
#include "Input.h"

Light::Light(glm::vec3 position, glm::vec3 color)
{
	lightPosition = position;
	lightColor = color;
}


Light::~Light()
{
}

void Light::input()
{
	if (Input::keyStates[1 + 320])
		lightPosition.z += 1.0f;
	if (Input::keyStates[9 + 320])
		lightPosition.z -= 1.0f;
}

void Light::update(glm::vec3& playerPosition)
{
	lightPosition.x = playerPosition.x;
	lightPosition.y = playerPosition.y;
}

void Light::setPosition(glm::vec3 position)
{
	this->lightPosition = position;
}