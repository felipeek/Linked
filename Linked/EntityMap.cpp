#include "EntityMap.h"


EntityMap::EntityMap(Transform* transform, Mesh* mesh) : Entity(transform, mesh){}


EntityMap::~EntityMap()
{
	if (transform != NULL)
		delete transform;
	if (mesh != NULL)
		delete mesh;
}

void EntityMap::render(MapShader* shader, Light* light)
{
	shader->useShader();
	shader->update(transform, light);
	mesh->renderMap();
	shader->stopShader();
}