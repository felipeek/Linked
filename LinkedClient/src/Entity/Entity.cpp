#include "Entity.h"
#include "Monster.h"
#include "Light.h"

Entity::Entity(Transform* transform, Mesh* mesh, Texture* texture)
{
	this->transform = transform;
	this->mesh = mesh;
	this->texture = texture;
}

Entity::~Entity()
{
	if (transform != NULL)
		delete transform;
	//if (mesh != NULL)
		//delete mesh;
	//if (texture != NULL)
		//delete texture;
}

Transform* Entity::getTransform()
{
	return transform;
}

void Entity::render(Shader* shader)
{
	shader->useShader();
	// Set shader data
	shader->setEntity(this);

	// Update shader
	bindTextures();
	shader->update();

	// Render
	mesh->render();
	shader->stopShader();
}

Texture* Entity::getTexture()
{
	return texture;
}

void Entity::setTexture(Texture* texture)
{
	this->texture = texture;
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

void Entity::bindTextures()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->textureID);
}