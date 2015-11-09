#pragma once
#include "IndexedModel.h"

#define ANGLE 30.0f

class Primitive
{
public:
	Primitive();
	virtual ~Primitive();
};

class Quad : public Primitive
{
public:
	Quad(glm::vec3 center, float sizeX, float sizeY);
	Quad(glm::vec3& topLeftCorner, glm::vec3& bottomRightCorner);
	Quad(glm::vec3 center, float sizeX, float sizeY, int textureNumRows, int textureIndex);
	virtual ~Quad();
	IndexedModel* getIndexedModel();

	float getTextureIndex();
	float getTextureNumRows();
	glm::vec2& getTextureOffset();

	void setIndex(int i);
	int getIndex();
private:
	float index;
	float numRows;
	glm::vec2 offset;

	void calcAtlas();

	IndexedModel* model;
};