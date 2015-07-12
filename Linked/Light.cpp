#include "Light.h"


Light::Light(glm::vec3 position, glm::vec3 color)
{
	lightPosition = position;
	lightColor = color;
}


Light::~Light()
{
}
