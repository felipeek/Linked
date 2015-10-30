#include "Primitive.h"
#include <glm\gtc\matrix_transform.hpp>

Primitive::Primitive()
{
}


Primitive::~Primitive()
{
}

Quad::Quad(glm::vec3 center, float sizeX, float sizeY) : Quad(center, sizeX, sizeY, 1, 0){}

Quad::Quad(glm::vec3 center, float sizeX, float sizeY, int textureNumRows, int textureIndex)
{
	model = new IndexedModel();

	this->index = (float)textureIndex;
	this->numRows = (float)textureNumRows;

	calcAtlas();

	glm::vec3 topL = glm::vec3(center.x - sizeX, center.y + sizeY, 0);	// 0
	glm::vec3 topR = glm::vec3(center.x + sizeX, center.y + sizeY, 0);	// 1
	glm::vec3 botL = glm::vec3(center.x - sizeX, center.y - sizeY, 0);	// 2
	glm::vec3 botR = glm::vec3(center.x + sizeX, center.y - sizeY, 0);	// 3

	model->getPositions()->push_back(topL);
	model->getPositions()->push_back(topR);
	model->getPositions()->push_back(botL);
	model->getPositions()->push_back(botR);

	glm::vec3 normal = glm::vec3(glm::rotate(glm::mat4(), -ANGLE, glm::vec3(1, 0, 0)) * glm::vec4(0,0,1,1));
	model->getNormals()->push_back(normal);
	model->getNormals()->push_back(normal);
	model->getNormals()->push_back(normal);
	model->getNormals()->push_back(normal);

	model->getTexCoords()->push_back(glm::vec2(0, 1));
	model->getTexCoords()->push_back(glm::vec2(1, 1));
	model->getTexCoords()->push_back(glm::vec2(0, 0));
	model->getTexCoords()->push_back(glm::vec2(1, 0));

	model->getIndices()->push_back(0);
	model->getIndices()->push_back(3);
	model->getIndices()->push_back(1);
	model->getIndices()->push_back(0);
	model->getIndices()->push_back(2);
	model->getIndices()->push_back(3);
}

void Quad::calcAtlas()
{
	int column = (int)index % (int)numRows;
	offset.x = (float)column / (float)numRows;
	int row = (int)index / (int)numRows;
	offset.y = (float)row / (float)numRows;
}

Quad::~Quad()
{
	delete model;
}

float Quad::getTextureIndex()
{
	return index;
}

float Quad::getTextureNumRows()
{
	return numRows;
}

glm::vec2& Quad::getTextureOffset()
{
	return offset;
}

void Quad::setIndex(int i)
{
	this->index = (float)i;
	calcAtlas();
}

IndexedModel* Quad::getIndexedModel()
{
	return model;
}