#include "Entity.h"

Entity::Entity(Transform* transform, Mesh* mesh)
{
	this->transform = transform;
	this->mesh = mesh;
}

Entity::~Entity()
{
	if (transform != NULL)
		delete transform;
	if (mesh != NULL)
		delete mesh;
}

Transform* Entity::getTransform()
{
	return transform;
}

void Entity::render(Shader* shader)
{
	shader->useShader();
	shader->update(transform, mesh->getTexture0());
	mesh->render();
	shader->stopShader();
}

Texture* Entity::getTexture()
{
	return mesh->getTexture0();
}

void Entity::setTransform(Transform* transform)
{
	this->transform = transform;
}

void Entity::setMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

Mesh* Entity::getMesh()
{
	return mesh;
}