#include "Game.h"
#include "MapTerrainImageLoader.h"

#include "MapEntityImageLoader.h"
#include <string>
#include <iostream>
#include <cstdlib> 

#include "Mesh.h"
#include "Grid.h"
#include "Input.h"

#include "Map.h"
#include "Display.h"

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
	// Câmera luz e shaders
	this->camera = new Camera(glm::vec3(0, 0, 50), glm::vec3(0, 0, 0), 70.0f, (float)windowsWidth / windowsHeight, 0.1f, 2500.0f);
	this->light = new Light(glm::vec3(100, 500, 10), glm::vec3(1, 1, 1));
	this->shader = new PrimitiveShader("./shaders/normalshader", camera);
	this->mapShader = new MapShader("./shaders/mapshader", camera);
	
	// Criação do player
	Mesh* playerMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 0.28f, 0.6f), new Texture("./res/Textures/clown.png"));
	Entity* player = new Entity(new Transform(glm::vec3(70, 980, 1.3f), 40, glm::vec3(1, 0, 0), glm::vec3(2, 2, 2)), playerMesh);
	entities.push_back(player);

	// Criação do Mapa
	std::string mapPath = "./res/Maps/maze.png";
	this->map = new Map(mapPath, mapPath, 3);
	Mesh* mapMesh = new Mesh(new Grid(1024, this->map),
		new Texture("./res/Maps/stonePath.png"),
		new Texture("./res/Maps/mountain.jpg"),
		new Texture("./res/Maps/water.jpg"),
		new Texture("./res/Maps/grassTex_00000.png"),
		new Texture(mapPath)
		);
	this->entityMap = new EntityMap(new Transform(), mapMesh);

	lastTime = 0;

	// Movimento
	playerMovement = new PlayerMovement(this->map, player);
}

Game::~Game()
{
	delete shader;
	delete camera;
	delete playerMovement;
}

void Game::render()
{
	entityMap->render(mapShader, light);

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
	glm::vec3 camPos = glm::vec3(entities[0]->getTransform()->getPosition().x, entities[0]->getTransform()->getPosition().y-10, 15);
	light->lightPosition.x = entities[0]->getTransform()->getPosition().x;
	light->lightPosition.y = entities[0]->getTransform()->getPosition().y;

	camera->setCamPosition(camPos);
	camera->setCamOrientation(camOri);
}

float charRot = 0;

void Game::input()
{
	playerMovement->inputPlayerMovement();

	if (Input::keyStates['8'])
		light->lightPosition.y += 1.0f;
	if (Input::keyStates['2'])
		light->lightPosition.y -= 1.0f;
	if (Input::keyStates['6'])
		light->lightPosition.x += 1.0f;
	if (Input::keyStates['4'])
		light->lightPosition.x -= 1.0f;
	if (Input::keyStates['1'])
		light->lightPosition.z += 1.0f;
	if (Input::keyStates['9'])
		light->lightPosition.z -= 1.0f;

	if (Input::keyStates['r'])
	{
		charRot += 0.1f;
		entities[0]->getTransform()->rotate(charRot, glm::vec3(1, 0, 0));
		std::cout << charRot << std::endl;
	}
	if (Input::keyStates['f'])
	{
		charRot -= 0.1f;
		entities[0]->getTransform()->rotate(charRot, glm::vec3(1, 0, 0));
		std::cout << charRot << std::endl;
	}

	if (Input::keyStates['t'])
	{
		

		double now = Time::getTime();
		double delta = now - lastTime;
		
		if (delta >= 0.3)
		{
			int randomNumberX;
			int randomNumberY;
			glm::vec3 newPos;
			do{
				randomNumberX = std::rand() % 1024;
				randomNumberY = std::rand() % 1024;
				newPos = glm::vec3((float)randomNumberX, (float)randomNumberY, 0);
			} while (map->getMapCoordinateForPlayerMovement(newPos).terrain != NORMAL_FLOOR);

			lastTime = Time::getTime();
			entities[0]->getTransform()->translate(newPos.x, newPos.y, entities[0]->getTransform()->getPosition().z);
		}
	}

	if (Input::keyStates['l'])
	{
		double now = Time::getTime();
		if (now - lastTime >= 0.3)
		{
			if (Mesh::drawForm == GL_TRIANGLES)
				Mesh::drawForm = GL_LINES;
			else
				Mesh::drawForm = GL_TRIANGLES;
			lastTime = Time::getTime();
		}
	}

}
