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

Quad::Quad(glm::vec3& topLeftCorner, glm::vec3& bottomRightCorner)
{
	model = new IndexedModel();

	this->index = 0.0f;
	this->numRows = 1.0f;

	glm::vec3 bottomLeft = glm::vec3(topLeftCorner.x, bottomRightCorner.y, topLeftCorner.z);
	glm::vec3 topRight = glm::vec3(bottomRightCorner.x, topLeftCorner.y, bottomRightCorner.z);

	model->positions.push_back(topLeftCorner);
	model->positions.push_back(topRight);
	model->positions.push_back(bottomLeft);
	model->positions.push_back(bottomRightCorner);

	glm::vec3 normal = glm::vec3(0, 1, 0);
	model->normals.push_back(normal);
	model->normals.push_back(normal);
	model->normals.push_back(normal);
	model->normals.push_back(normal);

	model->texCoords.push_back(glm::vec2(0, 0));	// 0
	model->texCoords.push_back(glm::vec2(1, 0));	// 1
	model->texCoords.push_back(glm::vec2(0, 1));	// 2
	model->texCoords.push_back(glm::vec2(1, 1));	// 3

	model->indices.push_back(0);
	model->indices.push_back(3);
	model->indices.push_back(1);
	model->indices.push_back(0);
	model->indices.push_back(2);
	model->indices.push_back(3);
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
	if (model)
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

int Quad::getIndex()
{
	return this->index;
}

IndexedModel* Quad::getIndexedModel()
{
	return model;
}