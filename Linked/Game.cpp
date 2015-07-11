#include "Game.h"
#include "MapObjectImageLoader.h"

#include "MapHeightImageLoader.h"
#include <string>
#include <iostream>

#include "Mesh.h"
#include "Primitive.h"
#include "PrimitiveShader.h"
#include "Input.h"
#include "Entity.h"

#include "Camera.h"

#include "Map.h"


Mesh* mesh;
Mesh* mesh2;
PrimitiveShader* shader;
Entity* entity;
Camera* camera;


//Game::Game()
//{
//	printCoordinate(0, 0);
//	printCoordinate(1, 0);
//	printCoordinate(2, 0);
//	printCoordinate(0, 1);
//	printCoordinate(1, 1);
//	printCoordinate(2, 1);
//
//	std::string shaderPath = "./shaders/normalshader";
//	mesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 0.3f, 0.3f), new Texture("./res/Textures/predio.jpg"));
//	mesh2 = new Mesh(new Quad(glm::vec3(0, 0, 0), 0.5f, 0.5f), new Texture("./res/imagem_hoshoyo2.png"));
//	entity = new Entity(new Transform(), mesh2);
//}

void Game::printCoordinate(int x, int y)
{
	std::string objectMapPath = "./res/Maps/objectmap.png";
	std::string heightMapPath = "./res/Maps/heightmap.png";
	std::string enumr = "teste";
	Map myOwnMap = Map(objectMapPath, heightMapPath, 3);
	MapCoordinate coord = myOwnMap.getMapCoordinate(glm::vec3(x, y, 0));
	switch (coord.object)
	{
	case NORMAL_FLOOR: enumr = "NORMAL_FLOOR"; break;
	case BLOCKED: enumr = "BLOCKED"; break;
	case HOLE: enumr = "HOLE"; break;
	case SPIKES: enumr = "SPIKES"; break;
	case FIRE: enumr = "FIRE"; break;
	case MUD: enumr = "MUD"; break;
	case SLIPPERY: enumr = "SLIPPERY"; break;
	default:
		enumr = "nenhum"; break;
	}
	std::cout << enumr << std::endl << std:: endl;
}

Game::Game(int windowsWidth, int windowsHeight)
{
	std::string shaderPath = "./shaders/normalshader";
	mesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 0.3f, 0.3f), new Texture("./res/Textures/predio.jpg"));
	entity = new Entity(new Transform(), mesh);
	camera = new Camera(glm::vec3(0,0,1), glm::vec3(0,0,0), 70.0f, (float)windowsWidth/windowsHeight, 0.1, 500.0f);
	shader = new PrimitiveShader(shaderPath, camera);
	printCoordinate(0, 0);
	printCoordinate(1, 0);
	printCoordinate(2, 0);
	printCoordinate(0, 1);
	printCoordinate(1, 1);
	printCoordinate(2, 1);
}

Game::~Game()
{
	delete shader;
	delete mesh;
}

void Game::render()
{
	input();
	entity->render(shader);
}

void Game::input()
{
	float x = 0.001f;
	if (Input::keyStates['w'])
		entity->getTransform()->incTranslate(0, x, 0);

	if (Input::keyStates['s'])
		entity->getTransform()->incTranslate(0, -x, 0);

	if (Input::keyStates['a'])
		entity->getTransform()->incTranslate(-x, 0, 0);

	if (Input::keyStates['d'])
		entity->getTransform()->incTranslate(x, 0, 0);

	if (Input::keyStates['r'])
		entity->getTransform()->incRotateY(0.0001f);
}
