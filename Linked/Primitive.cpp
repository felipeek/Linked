#include "Primitive.h"


Primitive::Primitive()
{
}


Primitive::~Primitive()
{
}

Quad::Quad(glm::vec3 center, float sizeX, float sizeY)
{
	model = new IndexedModel();

	glm::vec3 topL = glm::vec3(center.x - sizeX, center.y + sizeY, 0);	// 0
	glm::vec3 topR = glm::vec3(center.x + sizeX, center.y + sizeY, 0);	// 1
	glm::vec3 botL = glm::vec3(center.x - sizeX, center.y - sizeY, 0);	// 2
	glm::vec3 botR = glm::vec3(center.x + sizeX, center.y - sizeY, 0);	// 3

	model->positions.push_back(topL);
	model->positions.push_back(topR);
	model->positions.push_back(botL);
	model->positions.push_back(botR);

	model->normals.push_back(glm::vec3(0, 1, 0));
	model->normals.push_back(glm::vec3(0, 1, 0));
	model->normals.push_back(glm::vec3(0, 1, 0));
	model->normals.push_back(glm::vec3(0, 1, 0));

	model->texCoords.push_back(glm::vec2(0, 1));
	model->texCoords.push_back(glm::vec2(1, 1));
	model->texCoords.push_back(glm::vec2(0, 0));
	model->texCoords.push_back(glm::vec2(1, 0));

	model->indices.push_back(0);
	model->indices.push_back(3);
	model->indices.push_back(1);
	model->indices.push_back(0);
	model->indices.push_back(2);
	model->indices.push_back(3);
}

Quad::~Quad()
{

}

IndexedModel* Quad::getIndexedModel()
{
	return model;
}