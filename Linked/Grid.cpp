#include "Grid.h"
#include "Map.h"
#include <cmath>
#include <iostream>

Grid::Grid(int blockSize, Map* map)
{
	
	this->blockSize = blockSize;
	for (int i = 0; i < blockSize; i += 2)
	{
		for (int j = 0; j < blockSize; j += 2)
		{
			float height = 2;
			float height1 = 0;
			float height2 = 0;
			float height3 = 0;
			float height4 = 0;
			if (map->getMapTerrainForCoordinate(glm::vec3(j, i, 0)) == BLOCKED)
				height1 = height;
			if (map->getMapTerrainForCoordinate(glm::vec3(j, i + 1, 0)) == BLOCKED)
				height2 = height;
			if (map->getMapTerrainForCoordinate(glm::vec3(j + 1, i, 0)) == BLOCKED)
				height3 = height;
			if (map->getMapTerrainForCoordinate(glm::vec3(j + 1, i + 1, 0)) == BLOCKED)
				height4 = height;
			if (map->getMapTerrainForCoordinate(glm::vec3(j, i, 0)) == WATER)
				height1 = -height;
			if (map->getMapTerrainForCoordinate(glm::vec3(j, i + 1, 0)) == WATER)
				height2 = -height;
			if (map->getMapTerrainForCoordinate(glm::vec3(j + 1, i, 0)) == WATER)
				height3 = -height;
			if (map->getMapTerrainForCoordinate(glm::vec3(j + 1, i + 1, 0)) == WATER)
				height4 = -height;

			glm::vec3 pos1 = glm::vec3(j, i, height1);
			glm::vec3 pos2 = glm::vec3(j, i + 1, height2);
			glm::vec3 pos3 = glm::vec3(j + 1, i, height3);
			glm::vec3 pos4 = glm::vec3(j + 1, i + 1, height4);

			// Positions
			indexedModel.positions.push_back(pos1);
			indexedModel.positions.push_back(pos2);
			indexedModel.positions.push_back(pos3);
			indexedModel.positions.push_back(pos4);

			// TexCoords

			//indexedModel.texCoords.push_back(glm::vec2(pos1.x / blockSize*30.0f, pos1.y / blockSize*30.0f));
			//indexedModel.texCoords.push_back(glm::vec2(pos2.x / blockSize*30.0f, pos2.y / blockSize*30.0f));
			//indexedModel.texCoords.push_back(glm::vec2(pos3.x / blockSize*30.0f, pos3.y / blockSize*30.0f));
			//indexedModel.texCoords.push_back(glm::vec2(pos4.x / blockSize*30.0f, pos4.y / blockSize*30.0f));
			indexedModel.texCoords.push_back(glm::vec2(pos1.x / blockSize, pos1.y / blockSize));
			indexedModel.texCoords.push_back(glm::vec2(pos2.x / blockSize, pos2.y / blockSize));
			indexedModel.texCoords.push_back(glm::vec2(pos3.x / blockSize, pos3.y / blockSize));
			indexedModel.texCoords.push_back(glm::vec2(pos4.x / blockSize, pos4.y / blockSize));

			// Normals
			//indexedModel.normals.push_back(glm::vec3(0, 1, 0));
			//indexedModel.normals.push_back(glm::vec3(0, 1, 0));
			//indexedModel.normals.push_back(glm::vec3(0, 1, 0));
			//indexedModel.normals.push_back(glm::vec3(0, 1, 0));
		}
	}
	// Calc Normals
	int indexPos = 0;
	for (int i = 0; i < blockSize; i += 2)
	{
		for (int j = 0; j < blockSize; j += 2)
		{
			indexedModel.normals.push_back(calculateNormal(indexedModel.positions[indexPos]));
			indexedModel.normals.push_back(calculateNormal(indexedModel.positions[indexPos + 1]));
			indexedModel.normals.push_back(calculateNormal(indexedModel.positions[indexPos + 2]));
			indexedModel.normals.push_back(calculateNormal(indexedModel.positions[indexPos + 3]));
			indexPos += 4;
		}
	}

	for (int i = 0, prevFirst = 0, newFirst = 0, deslocamento = 0, ultimo = 0; i < blockSize - 1; i++)
	{
		for (int j = 0, aux = 0; j < blockSize - 1; j++)
		{
			indexedModel.indices.push_back(deslocamento + j + 3 + aux);
			indexedModel.indices.push_back(deslocamento + j + 1 + aux);
			indexedModel.indices.push_back(newFirst + j + aux);
			indexedModel.indices.push_back(newFirst + j + aux);
			indexedModel.indices.push_back(newFirst + j + 2 + aux);
			indexedModel.indices.push_back(deslocamento + j + 3 + aux);
			if (j == 0)
				prevFirst = deslocamento + j + 1 + aux;
			ultimo = deslocamento + j + 3 + aux;
			aux = j + 1;
		}
		newFirst = (prevFirst + 1) - ((prevFirst + 1) - prevFirst);
		if (i % 2 == 0)
			deslocamento = ultimo;
		else
			deslocamento = ultimo - ((blockSize - 2) * 2 - 1) - 3;
	}
}

glm::vec3 Grid::calculateNormal(glm::vec3& position)
{
	float heightL = getHeight(position.x - 1, position.y);
	float heightR = getHeight(position.x + 1, position.y);
	float heightD = getHeight(position.x, position.y - 1);
	float heightU = getHeight(position.x, position.y + 1);
	glm::vec3 normal = glm::normalize(glm::vec3(heightL - heightR, heightD - heightU, 2));
	return normal;
}

float Grid::getHeight(float x, float y)
{
	unsigned int index = 0;
	if ((int)y % 2 == 0)
	{
		index = (unsigned int)(blockSize * y + 2 * x);
	}
	else
	{
		index = (unsigned int)(((blockSize * (y - 1)) + 1) + 2 * x);
	}
	glm::vec3 position;
	if (index < indexedModel.positions.size())
		position = indexedModel.positions[index];
	else
		position = glm::vec3(0, 0, 0);

	return position.z;
}

Grid::~Grid()
{
}