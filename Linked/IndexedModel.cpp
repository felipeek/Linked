#include "IndexedModel.h"

using namespace std;

vector<glm::vec3>* IndexedModel::getPositions()
{
	return &positions;
}
vector<glm::vec2>* IndexedModel::getTexCoords()
{
	return &texCoords;
}
vector<glm::vec3>* IndexedModel::getNormals()
{
	return &normals;
}
vector<unsigned int>* IndexedModel::getIndices()
{
	return &indices;
}

// ObjModel

ObjModel::ObjModel(string fileName)
{
	loadObjFile(fileName);
}

ObjModel::~ObjModel(){}

IndexedModel* ObjModel::toIndexedModel()
{
	IndexedModel* result = new IndexedModel();

	int currentVertexIndex = 0;
	for (int i = 0; i < indices.size(); i++)
	{
		ObjIndex currentIndex = indices[i];
		glm::vec3 currentPosition = positions[currentIndex.vertexIndex];
		glm::vec2 currentTexCoord = texCoords[currentIndex.texCoordIndex];
		glm::vec3 currentNormal = normals[currentIndex.normalIndex];

		result->getPositions()->push_back(currentPosition);
		result->getTexCoords()->push_back(currentTexCoord);
		result->getNormals()->push_back(currentNormal);
		result->getIndices()->push_back(i);
		currentVertexIndex++;
	}

	return result;
}

void ObjModel::loadObjFile(string fileName)
{
	ifstream objFile(fileName);
	string line;
	while (!objFile.eof())
	{
		getline(objFile, line);

		if (line[0] == 'v' && line[1] == ' ')
			positions.push_back(parseVertex(line));
		if (line[0] == 'v' && line[1] == 't')
			texCoords.push_back(parseTexCoord(line));
		if (line[0] == 'v' && line[1] == 'n')
			normals.push_back(parseNormal(line));
		if (line[0] == 'f' && line[1] == ' ')
			parseTriangles(line);
	}
	objFile.close();
}

glm::vec3 ObjModel::parseVertex(string& line)
{
	float x, y, z;
	int i, beginIndex = 2;
	if (line[beginIndex] == ' ')
		beginIndex++;
	for (i = beginIndex; line[i] != ' '; i++);
	x = std::stof(line.substr(beginIndex, i));
	beginIndex = ++i;
	for (i = beginIndex; line[i] != ' '; i++);
	y = std::stof(line.substr(beginIndex, i));
	beginIndex = ++i;
	for (i = beginIndex; i < line.size() - 1 && line[i] != '\n' && line[i] != ' '; i++);
	z = std::stof(line.substr(beginIndex, i));
	//std::cout << "Vertice: " << x << " " << y << " " << z << std::endl;
	return glm::vec3(x, y, z);
}

glm::vec2 ObjModel::parseTexCoord(string& line)
{
	float x, y;
	int i, beginIndex = 3;
	for (i = beginIndex; line[i] != ' '; i++);
	x = std::stof(line.substr(beginIndex, i));
	beginIndex = ++i;
	for (i = beginIndex; i < line.size() - 1 && line[i] != '\n' && line[i] != ' '; i++);
	y = std::stof(line.substr(beginIndex, i));
	//std::cout << "Texture: " << x << " " << y << std::endl;
	return glm::vec2(x, y);
}

glm::vec3 ObjModel::parseNormal(string& line)
{
	float x, y, z;
	int i, beginIndex = 3;
	for (i = beginIndex; line[i] != ' '; i++);
	x = std::stof(line.substr(beginIndex, i));
	beginIndex = ++i;
	for (i = beginIndex; line[i] != ' '; i++);
	y = std::stof(line.substr(beginIndex, i));
	beginIndex = ++i;
	for (i = beginIndex; i < line.size() - 1 && line[i] != '\n' && line[i] != ' '; i++);
	z = std::stof(line.substr(beginIndex, i));
	//std::cout << "Normal: " << x << " " << y << " " << z << std::endl;
	return glm::vec3(x, y, z);
}

void ObjModel::parseTriangles(string& line)
{
	int i;
	string vdata[3];
	int vec[3][3];

	// Faz o parse da linha f int/int/int int/int/int int/int/int

	for (int j = 0, first = 2; j < 3; j++)
	{
		for (i = first; line[i] != '/'; i++);
		int v = stoi(line.substr(first, i));

		first = ++i;
		for (i = first; line[i] != '/'; i++);
		int t = stoi(line.substr(first, i));

		first = ++i;
		for (i = first; line[i] != ' ' && i != line.length() - 1 && line[i] != '\n'; i++);
		int n = stoi(line.substr(first, i));
		first = ++i;

		vec[j][0] = v;
		vec[j][1] = t;
		vec[j][2] = n;
	}

	ObjIndex *result1 = new ObjIndex();
	result1->vertexIndex = vec[0][0] - 1;
	result1->texCoordIndex = vec[0][1] - 1;
	result1->normalIndex = vec[0][2] - 1;

	ObjIndex *result2 = new ObjIndex();
	result2->vertexIndex = vec[1][0] - 1;
	result2->texCoordIndex = vec[1][1] - 1;
	result2->normalIndex = vec[1][2] - 1;

	ObjIndex *result3 = new ObjIndex();
	result3->vertexIndex = vec[2][0] - 1;
	result3->texCoordIndex = vec[2][1] - 1;
	result3->normalIndex = vec[2][2] - 1;

	indices.push_back(*result1);
	indices.push_back(*result2);
	indices.push_back(*result3);
}
