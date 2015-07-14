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
	void renderMap();
	float reflectivity;
	float glossiness;
	static GLuint drawForm;
private:

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



