#include "Mesh.h"
#include "Shader.h"
#include <iostream>

GLuint Mesh::drawForm = GL_TRIANGLES;

Mesh::Mesh(std::string fileName, Texture* texture, float reflectivity, float glossiness)
{
	ObjModel model(fileName);
	IndexedModel *indexedModel = model.toIndexedModel();
	this->texture0 = texture;
	this->reflectivity = reflectivity;
	this->glossiness = glossiness;

	genVAO();
	genVBOS(indexedModel);
	indicesSize = indexedModel->getIndices()->size() * sizeof(float);
	genIndexBuffer(indexedModel);
}

Mesh::Mesh(Quad* quad, Texture* texture)
{
	this->texture0 = texture;
	this->reflectivity = 0;
	this->glossiness = 0;
	genVAO();
	genVBOS(quad->getIndexedModel());
	indicesSize = quad->getIndexedModel()->getIndices()->size() * sizeof(float);
	genIndexBuffer(quad->getIndexedModel());
}

Mesh::Mesh(Grid* grid, Texture* texture0, Texture* texture1, Texture* texture2, Texture* blendMap)
{
	this->texture0 = texture0;
	this->texture1 = texture1;
	this->texture2 = texture2;
	this->blendMap = blendMap;

	this->reflectivity = 0;
	this->glossiness = 0;
	genVAO();
	genVBOS(grid->getIndexedModel());
	indicesSize = grid->getIndexedModel()->getIndices()->size() * sizeof(float);
	genIndexBuffer(grid->getIndexedModel());
}

Mesh::~Mesh()
{
	if (VertexArrayID != NULL)
		glDeleteVertexArrays(1, &VertexArrayID);
	if (VertexBufferID != NULL)
		glDeleteBuffers(1, &VertexBufferID);
	if (NormalsBufferID != NULL)
		glDeleteBuffers(1, &NormalsBufferID);
	if (TextureBufferID != NULL)
		glDeleteBuffers(1, &TextureBufferID);
	if (IndexBufferID != NULL)
		glDeleteBuffers(1, &IndexBufferID);
}

void Mesh::genVAO()
{
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
}

void Mesh::genVBOS(IndexedModel* iModel)
{
	// Vertex Data
	glGenBuffers(1, &VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, iModel->getPositions()->size() * sizeof(float) * 3, &iModel->positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDisableVertexAttribArray(0);


	// Normals Data
	glGenBuffers(1, &NormalsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, NormalsBufferID);
	glBufferData(GL_ARRAY_BUFFER, iModel->getNormals()->size() * sizeof(float) * 3, &iModel->normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDisableVertexAttribArray(1);

	// Texture Data
	glGenBuffers(1, &TextureBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, TextureBufferID);
	glBufferData(GL_ARRAY_BUFFER, iModel->getTexCoords()->size() * sizeof(float) * 2, &iModel->texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glDisableVertexAttribArray(2);
}

void Mesh::genIndexBuffer(IndexedModel* iModel)
{
	glGenBuffers(1, &IndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, &iModel->indices[0], GL_STATIC_DRAW);
}

void Mesh::render()
{
	//shader.update(transform);
	glBindVertexArray(VertexArrayID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0->textureID);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, NormalsBufferID);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, TextureBufferID);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawElements(drawForm, indicesSize / sizeof(float), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

void Mesh::renderMap()
{
	//shader.update(transform);
	glBindVertexArray(VertexArrayID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0->textureID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1->textureID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture2->textureID);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, blendMap->textureID);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, NormalsBufferID);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, TextureBufferID);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawElements(drawForm, indicesSize / sizeof(float), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}
