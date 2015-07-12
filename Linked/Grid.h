#pragma once
#include "Primitive.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "IndexedModel.h"
#include "Map.h"

class Grid : public Primitive
{
public:
	Grid(int blockSize, Map* map);
	~Grid();
	inline IndexedModel* getIndexedModel(){ return &indexedModel; }
private:
	IndexedModel indexedModel;
	int blockSize;
};

