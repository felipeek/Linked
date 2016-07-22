#include "Grid.h"
#include "Map.h"

Grid::Grid(int blockSize, Map* map)
	: blockSize(blockSize)
{
	int arraySize = blockSize * blockSize;
	// Preallocate
	indexedModel.positions.resize(arraySize);
	indexedModel.normals.resize(arraySize);
	indexedModel.texCoords.resize(arraySize);
	
	for (int i = 0, var = 0; i < blockSize; i += 2)
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
			indexedModel.positions[var] = pos1;
			indexedModel.positions[var + 1] = pos2;
			indexedModel.positions[var + 2] = pos3;
			indexedModel.positions[var + 3] = pos4;

			// TexCoords
			indexedModel.texCoords[var] = glm::vec2(pos1.x / blockSize, pos1.y / blockSize);
			indexedModel.texCoords[var + 1] = glm::vec2(pos2.x / blockSize, pos2.y / blockSize);
			indexedModel.texCoords[var + 2] = glm::vec2(pos3.x / blockSize, pos3.y / blockSize);
			indexedModel.texCoords[var + 3] = glm::vec2(pos4.x / blockSize, pos4.y / blockSize);

			var += 4;
		}
	}
	
	// Calc Normals
	
	int indexPos = 0;
	for (int i = 0; i < blockSize; i += 2)
	{
		for (int j = 0; j < blockSize; j += 2)
		{
			indexedModel.normals[indexPos] = calculateNormal(indexedModel.positions[indexPos]);
			indexedModel.normals[indexPos + 1] = calculateNormal(indexedModel.positions[indexPos + 1]);
			indexedModel.normals[indexPos + 2] = calculateNormal(indexedModel.positions[indexPos + 2]);
			indexedModel.normals[indexPos + 3] = calculateNormal(indexedModel.positions[indexPos + 3]);
			indexPos += 4;
		}
	}
	
	int halfblock = blockSize / 2.0f;
	indexedModel.indices.resize((halfblock * (blockSize - 1) + (halfblock - 1) * (blockSize - 1)) * 6);
	
	for (int i = 0, prevFirst = 0, newFirst = 0, deslocamento = 0, ultimo = 0, var = 0; i < blockSize - 1; i++)
	{
		for (int j = 0, aux = 0; j < blockSize - 1; j++)
		{
			indexedModel.indices[var + 0] = deslocamento + j + 3 + aux;
			indexedModel.indices[var + 1] = deslocamento + j + 1 + aux;
			indexedModel.indices[var + 2] = newFirst + j + aux;
			indexedModel.indices[var + 3] = newFirst + j + aux;
			indexedModel.indices[var + 4] = newFirst + j + 2 + aux;
			indexedModel.indices[var + 5] = deslocamento + j + 3 + aux;

			var += 6;

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