#pragma once
#include "Transform.h"
#include "Mesh.h"
#include "Shader.h"

class Entity
{
public:
	Entity(Transform* transform, Mesh* mesh);
	~Entity();
	Transform* getTransform();
	void render(Shader* shader);
	void renderMap(Shader* shader);
private:
	Transform* transform;
	Mesh* mesh;
};

