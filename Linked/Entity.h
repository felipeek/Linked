#pragma once
#include "Transform.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

class Entity
{
public:
	Entity(Transform* transform, Mesh* mesh);
	~Entity();
	Transform* getTransform();
	void setTransform(Transform* transform);
	virtual void render(Shader* shader);
	Texture* getTexture();
	void setMesh(Mesh* mesh);
	Mesh* getMesh(){
		return mesh;
	}
protected:
	Transform* transform;
	Mesh* mesh;
};

