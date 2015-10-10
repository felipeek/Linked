#pragma once
#include <vector>
#include <glm\glm.hpp>
#include <GL\glew.h>
#include "IndexedModel.h"

class Grid;
class Quad;

class Mesh
{
public:
	Mesh(std::string, float, float);
	Mesh(Quad* quad);
	Mesh(Grid* grid);
	~Mesh();
	void render();
	float reflectivity;
	float glossiness;
	static GLuint drawForm;

	// returns NULL if quad does not exist
	Quad* getQuad();

	GLuint getVertexArrayID();
	GLuint getVertexBufferID();
	GLuint getTextureBufferID();
	GLuint getNormalsBufferID();
	GLuint getIndexBufferID();
private:
	Quad* quad;
	Grid* grid;
	GLuint VertexArrayID;
	GLuint VertexBufferID;
	GLuint TextureBufferID;
	GLuint NormalsBufferID;
	GLuint IndexBufferID;

	std::vector<unsigned int> indices;

	void genVAO();
	void genVBOS(IndexedModel*);
	void genIndexBuffer(IndexedModel*);

	int indicesSize;
};



