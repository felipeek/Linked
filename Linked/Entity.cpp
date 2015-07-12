#include "Entity.h"


Entity::Entity(Transform* transform, Mesh* mesh)
{
	this->transform = transform;
	this->mesh = mesh;
}

Entity::~Entity()
{
	delete transform;
}

Transform* Entity::getTransform()
{
	return transform;
}

void Entity::render(Shader* shader)
{
	shader->useShader();
	shader->update(transform);
	mesh->render();
	shader->stopShader();
}

void Entity::renderMap(MapShader* shader, Light* light)
{
	shader->useShader();
	shader->update(transform, light);
	mesh->renderMap();
	shader->stopShader();
}