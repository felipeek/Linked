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
	if (Input::keyStates['8'])
		lightPosition.y += 1.0f;
	if (Input::keyStates['2'])
		lightPosition.y -= 1.0f;
	if (Input::keyStates['6'])
		lightPosition.x += 1.0f;
	if (Input::keyStates['4'])
		lightPosition.x -= 1.0f;
	if (Input::keyStates['1'])
		lightPosition.z += 1.0f;
	if (Input::keyStates['9'])
		lightPosition.z -= 1.0f;
}

void Light::update(glm::vec3& playerPosition)
{
	lightPosition.x = playerPosition.x;
	lightPosition.y = playerPosition.y;
}