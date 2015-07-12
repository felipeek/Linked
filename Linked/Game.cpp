#include "Game.h"
#include "MapTerrainImageLoader.h"

#include "MapEntityImageLoader.h"
#include <string>
#include <iostream>

#include "Mesh.h"
#include "Grid.h"
#include "Input.h"

#include "Map.h"
#include "Display.h"

MapShader* mapShader;
Entity* map;

/*void Game::printCoordinate(int x, int y)
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
}*/

Game::Game(int windowsWidth, int windowsHeight)
{
	std::string mapPath = "./res/Maps/officialmap.png";
	Mesh* mesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 0.3f, 0.3f), new Texture("./res/Textures/predio.jpg"));
	Mesh* mesh2 = new Mesh(new Grid(1024, new Map(mapPath, mapPath, 3)),
		new Texture("./res/Maps/path.png"),
		new Texture("./res/Maps/mountain.jpg"),
		new Texture("./res/Maps/water.jpg"),
		new Texture("./res/Maps/dirt.png"),
		new Texture(mapPath)
		);
	Entity* entity = new Entity(new Transform(0,0,10), mesh);
	map = new Entity(new Transform(), mesh2);
	map->getTransform()->rotate(-45, glm::vec3(1, 0, 0));
	this->camera = new Camera(glm::vec3(0,0,50), glm::vec3(0,0,0), 70.0f, (float)windowsWidth/windowsHeight, 0.1f, 500.0f);
	this->shader = new PrimitiveShader("./shaders/normalshader", camera);

	mapShader = new MapShader("./shaders/mapshader", camera);
	entities.push_back(entity);
}

Game::~Game()
{
	delete shader;
	delete camera;
}

void Game::render()
{
	map->renderMap(mapShader);
	for (Entity* e : entities)
	{
		try{
			e->render(shader);
		}
		catch (...){
			std::cerr << "Error rendering entity" << std::endl;
		}
	}
}

void Game::update()
{
	input();
	glm::vec3 camOri = glm::vec3(entities[0]->getTransform()->getPosition().x, entities[0]->getTransform()->getPosition().y, 0);
	glm::vec3 camPos = glm::vec3(entities[0]->getTransform()->getPosition().x, entities[0]->getTransform()->getPosition().y, 30);

	camera->setCamPosition(camPos);
	camera->setCamOrientation(camOri);
}

void Game::input()
{
	float speed = 100;

	if (Input::keyStates['w'])
		entities[0]->getTransform()->incTranslate(0, (float)Display::frameTime * speed, 0);

	if (Input::keyStates['s'])
		entities[0]->getTransform()->incTranslate(0, -(float)Display::frameTime * speed, 0);

	if (Input::keyStates['a'])
		entities[0]->getTransform()->incTranslate(-(float)Display::frameTime * speed, 0, 0);

	if (Input::keyStates['d'])
		entities[0]->getTransform()->incTranslate((float)Display::frameTime * speed, 0, 0);

	if (Input::keyStates['r'])
		entities[0]->getTransform()->incRotateY(0.0001f);

	if (Input::keyStates['l'])
	{
	if (Mesh::drawForm == GL_TRIANGLES)
		Mesh::drawForm = GL_LINES;
	else
		Mesh::drawForm = GL_TRIANGLES;
	}
}
