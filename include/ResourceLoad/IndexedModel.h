#pragma once
#include <glm\glm.hpp>
#include <vector>
#include <string>

struct ObjIndex{
public:
	int vertexIndex;
	int texCoordIndex;
	int normalIndex;
};

class IndexedModel
{
public:
	std::vector<glm::vec3>* getPositions();
	std::vector<glm::vec2>* getTexCoords();
	std::vector<glm::vec3>* getNormals();
	std::vector<unsigned int>* getIndices();
	//private:
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
};

class ObjModel{
public:
	ObjModel(std::string);
	~ObjModel();
	IndexedModel* toIndexedModel();
	std::vector<ObjIndex> indices;
private:
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;

	void loadObjFile(std::string);

	glm::vec3 parseVertex(std::string&);
	glm::vec2 parseTexCoord(std::string&);
	glm::vec3 parseNormal(std::string&);
	void parseTriangles(std::string&);
};
