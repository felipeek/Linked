#pragma once
#include "IndexedModel.h"

class Primitive
{
public:
	Primitive();
	~Primitive();
};

class Quad : public Primitive
{
public:
	Quad(glm::vec3 center, float sizeX, float sizeY);
	~Quad();
	IndexedModel* getIndexedModel();
private:
	IndexedModel* model;
};