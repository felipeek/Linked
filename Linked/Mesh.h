#pragma once
#include <vector>
#include <glm\glm.hpp>
#include <GL\glew.h>
#include "IndexedModel.h"
#include "Texture.h"
#include "Primitive.h"

class Mesh
{
public:
	Mesh(std::string, Texture*, float, float);
	Mesh(Quad* quad, Texture* texture);
	~Mesh();
	void render();
	float reflectivity;
	float glossiness;
	static GLuint drawForm;
private:

	GLuint VertexArrayID;
	GLuint VertexBufferID;
	GLuint TextureBufferID;
	GLuint NormalsBufferID;
	GLuint IndexBufferID;
	Texture *texture;

	std::vector<unsigned int> indices;

	void genVAO();
	void genVBOS(IndexedModel*);
	void genIndexBuffer(IndexedModel*);

	int indicesSize;
};



