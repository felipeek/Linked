#pragma once
#include "Transform.h"
#include "Mesh.h"
#include "Shader.h"
#include "MapShader.h"

class Entity
{
public:
	Entity(Transform* transform, Mesh* mesh);
	~Entity();
	Transform* getTransform();
	virtual void render(Shader* shader);
protected:
	Transform* transform;
	Mesh* mesh;
};

