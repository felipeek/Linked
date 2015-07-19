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
#include "GameEntityFactory.h"
#include "Projectile.h"

#include "Camera.h"
#include "PrimitiveShader.h"
#include "MapShader.h"
#include "Light.h"

#include "RangeAttack.h"

#include "Player.h"
#include "HPBar.h"

#include <string>
#include <iostream>
#include <cstdlib>

//#define DEBUG

HPBar* hpBar;

Game::Game(int windowsWidth, int windowsHeight)
{	
	// Câmera luz e shaders
	this->camera = new Camera(glm::vec3(0, 0, 50), glm::vec3(0, 0, 0), 70.0f, (float)windowsWidth / windowsHeight, 0.1f, 500.0f);
	this->light = new Light(glm::vec3(100, 500, 50), glm::vec3(1, 1, 1));
	this->shader = new PrimitiveShader("./shaders/normalshader", camera);
	this->mapShader = new MapShader("./shaders/mapshader", camera);
	
	// Criação do player
	Mesh* playerMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f));
	player = new Player(new Transform(glm::vec3(534, 500, 1.0f), 45, glm::vec3(1, 0, 0), glm::vec3(2, 2, 2)), playerMesh, new Texture("./res/Textures/hoshoyo.png", 2, 2));
	player->setMaximumHpBasis(1000);
	player->setHp(1000);
	player->setDefenseBasis(100);
	entities.push_back(player);
	//hpBar = new HPBar(100, player);
	
	// Criação do Mapa
	std::string mapPath = "./res/Maps/teste.png";
	std::string entitiesMapPath = "./res/Maps/entities.png";
	std::string monsterMapPath = "./res/Maps/monsters.png";

	this->monsterFactory = new MonsterFactory();
	this->gameEntityFactory = new GameEntityFactory();
	
	this->map = new Map(mapPath, entitiesMapPath, monsterMapPath, 3, monsterFactory, gameEntityFactory);
	Mesh* mapMesh = new Mesh(new Grid(MAP_SIZE, map));
	this->entityMap = new EntityMap(new Transform(), mapMesh,
		new Texture("./res/Maps/stonePath.png"),
		new Texture("./res/Maps/mountain.jpg"),
		new Texture("./res/Maps/water.jpg"),
		new Texture("./res/Maps/grassTex.png"),
		new Texture(mapPath));

	// Criação dos Monstros e das Entidades
	for (int i = 0; i < MAP_SIZE; i++)
		for (int j = 0; j < MAP_SIZE; j++)
		{
			MapCoordinate coordinate = map->getMapCoordinateForMapCreation(glm::vec3(i,j,0));
			Monster *monster = coordinate.mapMonster.monster;
			GameEntity *gameEntity = coordinate.mapGameEntity.gameEntity;
	
			if (coordinate.mapMonster.monsterExists)
			{
				if (!map->coordinateHasCollision(glm::vec3(i, j, 0)))
				{
					monster->getTransform()->translate((float)i, (float)j, 1.0f);
					monsters.push_back(monster);
				}
				else
					delete monster;
			}
			if (coordinate.mapGameEntity.gameEntityExists)
			{
				gameEntity->getTransform()->translate((float)i, (float)j, 0);
				gameEntities.push_back(gameEntity);
			}
		}

	/*for (int i = 0; i < monsters.size(); i++)
		std::cout << monsters[i]->getName() << std::endl;*/

	std::cout << "QUANTIDADE DE MONSTROS: " << monsters.size() << std::endl;

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
	if (this->map != NULL)
		delete map;
	for (Monster* monster : monsters)
		delete monster;
}

void Game::render()
{
	entityMap->render(mapShader, light);

	player->getHPBar()->quad->render(shader, light);

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
	for (Entity* e : gameEntities)
	{
		try{
			e->render(shader, light);
		}
		catch (...){
			std::cerr << "Error rendering entity" << std::endl;
		}
	}
}

bool playerDead = false;
unsigned int lastHp = 0;

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

	// Player input & update
	player->update();
	player->input();
	
	for (Monster* monster : monsters)
		monster->update(map, player);
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
			} while (map->getMapCoordinateForEntityMovement(newPos).terrain != NORMAL_FLOOR);

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