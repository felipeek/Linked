#pragma once
#include <vector>
#include <glm\glm.hpp>
#include <GL\glew.h>
#include "IndexedModel.h"
#include "Texture.h"

class Grid;
class Quad;

class Mesh
{
public:
	Mesh(std::string, Texture*, float, float);
	Mesh(Quad* quad, Texture* texture);
	Mesh(Grid* grid, Texture*, Texture*, Texture*, Texture*, Texture*);
	~Mesh();
	void render();
	void renderMap();
	float reflectivity;
	float glossiness;
	static GLuint drawForm;
	Texture* getTexture0();
private:

	GLuint VertexArrayID;
	GLuint VertexBufferID;
	GLuint TextureBufferID;
	GLuint NormalsBufferID;
	GLuint IndexBufferID;

	Texture *texture0;
	Texture *texture1;
	Texture *texture2;
	Texture *texture3;
	Texture *blendMap;

	std::vector<unsigned int> indices;

	void genVAO();
	void genVBOS(IndexedModel*);
	void genIndexBuffer(IndexedModel*);

	int indicesSize;
};



