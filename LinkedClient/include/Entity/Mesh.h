#pragma once
#include <vector>
#include <glm\glm.hpp>
#include <GL\glew.h>

class Grid;
class Quad;
class IndexedModel;

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
	static bool wireframe;
	static bool isGUI;

	// returns NULL if quad does not exist
	Quad* getQuad();

	GLuint getVertexArrayID();
	GLuint getVertexBufferID();
	GLuint getTextureBufferID();
	GLuint getNormalsBufferID();
	GLuint getIndexBufferID();

	int& getReferenceCount();
	void setReferenceCount(int refCount);
private:
	Quad* quad;
	Grid* grid;
	IndexedModel* indexedModel;
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

	int referenceCount;
};



