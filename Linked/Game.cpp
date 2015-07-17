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
#include "Projectile.h"

#include "Camera.h"
#include "PrimitiveShader.h"
#include "MapShader.h"
#include "Light.h"

#include "RangeAttack.h"

#include <string>
#include <iostream>
#include <cstdlib>

//#define DEBUG

Game::Game(int windowsWidth, int windowsHeight)
{	
	// Câmera luz e shaders
	this->camera = new Camera(glm::vec3(0, 0, 50), glm::vec3(0, 0, 0), 70.0f, (float)windowsWidth / windowsHeight, 0.1f, 2500.0f);
	this->light = new Light(glm::vec3(100, 500, 50), glm::vec3(1, 1, 1));
	this->shader = new PrimitiveShader("./shaders/normalshader", camera);
	this->mapShader = new MapShader("./shaders/mapshader", camera);
	
	// Criação do player
	Mesh* playerMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f));
	player = new Entity(new Transform(glm::vec3(534, 500, 1.0f), 45, glm::vec3(1, 0, 0), glm::vec3(2, 2, 2)), playerMesh, new Texture("./res/Textures/clownAtlas.png", 2, 2));
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
	std::string monsterMapPath = "./res/Maps/poringMap.png";
	this->monsterFactory = new MonsterFactory();
	this->monsterMap = new Map(mapPath, mapPath, monsterMapPath, 3, this->monsterFactory);
	
	//bool aux = false;
	
	for (int i = 0; i < MAP_SIZE; i++)
		for (int j = 0; j < MAP_SIZE; j++)
		{
			MapCoordinate coordinate = monsterMap->getMapCoordinateForMapCreation(glm::vec3(i,j,0));
			Monster *monster = coordinate.mapMonster.monster;
	
			if (coordinate.mapMonster.monsterExists == true)
			{
				//if (!aux)
				//{
				if (!MapTerrainImageLoader::isOfCollisionType(monsterMap->getMapCoordinateForPlayerMovement(glm::vec3(i, j, 0)).terrain))
				{
					monster->getTransform()->translate((float)i, (float)j, 1.0f);
					monsters.push_back(monster);
				}
				//aux = true;
				//}
			}
		}

	Mesh* houseMesh = new Mesh("./res/obj/house1.obj", 0, 0);
	Texture* houseTexture = new Texture("./res/obj/Textures/house1Texture1.png");
	Entity* house1 = new Entity(new Transform(glm::vec3(530, 500, 0), 90, glm::vec3(1, 0, 0), glm::vec3(0.6f, 0.6f, 0.6f)), houseMesh, houseTexture);
	house1->getTransform()->incRotateY(180);
	entities.push_back(house1);
		
	/*for (int i = 0; i < monsters.size(); i++)
		std::cout << monsters[i]->getName() << std::endl;*/

	std::cout << monsters.size() << std::endl;

	lastTime = 0;
	
	// Movimento
	playerMovement = new PlayerMovement(this->map, player);
	
	// Ataque à distancia
	rangeAttack = new RangeAttack(player, &attacks, &monsters, map);
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
			e->render(shader, light);
		}
		catch (...){
			std::cerr << "Error rendering entity" << std::endl;
		}
	}
	for (Entity* e : monsters)
	{
		try{
			e->render(shader, light);
		}
		catch (...){
			std::cerr << "Error rendering entity" << std::endl;
		}
	}
	for (Entity* e : attacks)
	{
		try{
			e->render(shader, light);
		}
		catch (...){
			std::cerr << "Error rendering entity" << std::endl;
		}
	}
}

void Game::update()
{
	// Game input
	input();

	// Camera input & update
	camera->input();
	camera->update(player->getTransform()->getPosition());

	// Light input & update
	light->input();
	light->update(player->getTransform()->getPosition());

	// RangeAttack input & update
	rangeAttack->input();
	rangeAttack->update();
	
	for (Monster* monster : monsters)
		monster->moveTo(player, map);
}

void Game::input()
{
	playerMovement->inputPlayerMovement();

#ifdef DEBUG
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
#endif
}