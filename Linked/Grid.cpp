#include "Grid.h"
#include <cmath>
#include <iostream>


Grid::Grid(int blockSize, Map* map)
{
	
	this->blockSize = blockSize;
	for (int i = 0; i < blockSize; i += 2)
	{
		for (int j = 0; j < blockSize; j += 2)
		{
			float height = 1.5;
			float height1 = 0;
			float height2 = 0;
			float height3 = 0;
			float height4 = 0;
			if (map->getMapCoordinate(glm::vec3(j, i, 0)).terrain == BLOCKED)
				height1 = height;
			if (map->getMapCoordinate(glm::vec3(j, i + 1, 0)).terrain == BLOCKED)
				height2 = height;
			if (map->getMapCoordinate(glm::vec3(j + 1, i, 0)).terrain == BLOCKED)
				height3 = height;
			if (map->getMapCoordinate(glm::vec3(j + 1, i + 1, 0)).terrain == BLOCKED)
				height4 = height;
			if (map->getMapCoordinate(glm::vec3(j, i, 0)).terrain == WATER)
				height1 = -height;
			if (map->getMapCoordinate(glm::vec3(j, i + 1, 0)).terrain == WATER)
				height2 = -height;
			if (map->getMapCoordinate(glm::vec3(j + 1, i, 0)).terrain == WATER)
				height3 = -height;
			if (map->getMapCoordinate(glm::vec3(j + 1, i + 1, 0)).terrain == WATER)
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
			indexedModel.normals.push_back(glm::vec3(0, 1, 0));
			indexedModel.normals.push_back(glm::vec3(0, 1, 0));
			indexedModel.normals.push_back(glm::vec3(0, 1, 0));
			indexedModel.normals.push_back(glm::vec3(0, 1, 0));
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


Grid::~Grid()
{
}