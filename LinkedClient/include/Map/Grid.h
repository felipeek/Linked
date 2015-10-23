#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "Primitive.h"
#include "IndexedModel.h"
class Map;

class Grid : public Primitive
{
public:
	Grid(int blockSize, Map* map);
	~Grid();
	inline IndexedModel* getIndexedModel(){ return &indexedModel; }
private:
	glm::vec3 calculateNormal(glm::vec3& position);
	float getHeight(float x, float y);
	IndexedModel indexedModel;
	int blockSize;
};

