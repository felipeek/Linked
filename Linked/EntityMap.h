#pragma once
#include "Entity.h"

class EntityMap : public Entity
{
public:
	EntityMap(Transform* transform, Mesh* mesh);
	~EntityMap();
	virtual void render(MapShader* shader, Light* light);
};

