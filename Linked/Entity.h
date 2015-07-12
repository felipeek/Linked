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
	void render(Shader* shader);
	void renderMap(MapShader* shader, Light* light);
private:
	Transform* transform;
	Mesh* mesh;
};

