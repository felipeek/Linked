#pragma once
#include "Transform.h"
#include "Mesh.h"
#include "Shader.h"
#include "MapShader.h"
#include "Texture.h"

class Entity
{
public:
	Entity(Transform* transform, Mesh* mesh);
	~Entity();
	Transform* getTransform();
	virtual void render(Shader* shader);
	Texture* getTexture();
protected:
	Transform* transform;
	Mesh* mesh;
};

