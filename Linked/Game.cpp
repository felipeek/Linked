#include "Game.h"

#include "Display.h"
#include "Time.h"
#include "Input.h"

#include "Map.h"
#include "MapTerrain.h"
#include "MapEntity.h"

#include "Mesh.h"
#include "Grid.h"
#include "EntityMap.h"
#include "PlayerMovement.h"
#include "MonsterFactory.h"

#include "Camera.h"
#include "PrimitiveShader.h"
#include "MapShader.h"
#include "Light.h"

#include <string>
#include <iostream>
#include <cstdlib>

float cameraDistance = 20;

Game::Game(int windowsWidth, int windowsHeight)
{	
	// Câmera luz e shaders
	this->camera = new Camera(glm::vec3(0, 0, 50), glm::vec3(0, 0, 0), 70.0f, (float)windowsWidth / windowsHeight, 0.1f, 2500.0f);
	this->light = new Light(glm::vec3(100, 500, 50), glm::vec3(1, 1, 1));
	this->shader = new PrimitiveShader("./shaders/normalshader", camera);
	this->mapShader = new MapShader("./shaders/mapshader", camera);
	
	// Criação do player
	//Mesh* playerMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 0.28f, 0.6f), new Texture("./res/Textures/clown.png"));
	Mesh* playerMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f));
	Entity* player = new Entity(new Transform(glm::vec3(70, 980, 1.0f), 45, glm::vec3(1, 0, 0), glm::vec3(2, 2, 2)), playerMesh, new Texture("./res/Textures/clownAtlas.png", 2, 2));
	entities.push_back(player);

	// Criação do Mapa
	std::string mapPath = "./res/Maps/teste.png";
	this->map = new Map(mapPath, mapPath, 3);
	Mesh* mapMesh = new Mesh(new Grid(MAP_SIZE, this->map));
	this->entityMap = new EntityMap(new Transform(), mapMesh,
		new Texture("./res/Maps/stonePath.png"),
		new Texture("./res/Maps/mountain.jpg"),
		new Texture("./res/Maps/water.jpg"),
		new Texture("./res/Maps/grassTex.png"),
		new Texture(mapPath));

	// Criação dos Monstros
	std::string monsterMapPath = "./res/Maps/monsters.png";
	this->monsterFactory = new MonsterFactory();
	this->monsterMap = new Map(mapPath, mapPath, monsterMapPath, 3, this->monsterFactory);

	for (int i = 0; i < MAP_SIZE; i++)
		for (int j = 0; j < MAP_SIZE; j++)
		{
			MapCoordinate coordinate = monsterMap->getMapCoordinateForMapCreation(glm::vec3(i,j,0));
			Monster *monster = coordinate.mapMonster.monster;

			if (coordinate.mapMonster.monsterExists == true)
			{
				monster->getTransform()->translate(i, j, 1.0f);
				entities.push_back(monster);
				monsters.push_back(monster);
			}
		}

	for (int i = 0; i < monsters.size(); i++)
		std::cout << monsters[i]->getName() << std::endl;

	lastTime = 0;

	// Movimento
	playerMovement = new PlayerMovement(this->map, player);
}

Game::~Game()
{
	delete shader;
	delete camera;
	delete playerMovement;
	if (monsterFactory != NULL)
		delete monsterFactory;
	if (this->monsterMap != NULL)
		delete monsterMap;
	for (Monster* monster : monsters)
		delete monster;
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
	glm::vec3 camPos = glm::vec3(entities[0]->getTransform()->getPosition().x, entities[0]->getTransform()->getPosition().y-10, cameraDistance);
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

	if (Input::keyStates['='])
	{
		cameraDistance += 1.0f;
	}

	if (Input::keyStates['-'])
	{
		if (cameraDistance > 10)
			cameraDistance -= 1.0f;
	}

	if (Input::attack)
	{
		glm::vec3 attackVector = Input::mouseAttack.attack(entities[0]->getTransform()->getPosition());
		float pz = entities[0]->getTransform()->getPosition().z;
		glm::vec3 projVector = attackVector;
		projVector.z = pz;
		//entities[0]->getTransform()->translate(attackVector.x, attackVector.y, pz);
		Texture* textureD = new Texture("./res/Textures/clownAtlas.png", 2, 2);
		Entity* entityD = new Entity(new Transform(projVector, 35, glm::vec3(1,0,0), glm::vec3(10,10,10)), entities[0]->getMesh(), textureD);
		entities.push_back(entityD);
	}
}

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
