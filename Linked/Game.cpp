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

#define SLIDE_FACTOR 100
#define SLIDE_SPEED_FACTOR 1.0

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

Map* mapM;
Light* light;
Entity* entity;

Game::Game(int windowsWidth, int windowsHeight)
{
	std::string mapPath = "./res/Maps/maze.png";
	Mesh* mesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 0.28f, 0.5f), new Texture("./res/Textures/clown.png"));
	light = new Light(glm::vec3(100, 500, 10), glm::vec3(1, 1, 1));

	mapM = new Map(mapPath, mapPath, 3);

	Mesh* mesh2 = new Mesh(new Grid(1024, mapM),
		new Texture("./res/Maps/stonePath.png"),
		new Texture("./res/Maps/mountain.jpg"),
		new Texture("./res/Maps/water.jpg"),
		new Texture("./res/Maps/dirt.png"),
		new Texture(mapPath)
		);
	entity = new Entity(new Transform(70,980,1.1f), mesh);
	entity->getTransform()->scale(3, 3, 3);
	entity->getTransform()->rotate(45, glm::vec3(1, 0, 0));
	map = new Entity(new Transform(), mesh2);
	this->camera = new Camera(glm::vec3(0,0,50), glm::vec3(0,0,0), 70.0f, (float)windowsWidth/windowsHeight, 0.1f, 2500.0f);
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
	map->renderMap(mapShader, light);
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
	glm::vec3 camPos = glm::vec3(entities[0]->getTransform()->getPosition().x, entities[0]->getTransform()->getPosition().y-10, 30);
	light->lightPosition.x = entity->getTransform()->getPosition().x;
	light->lightPosition.y = entity->getTransform()->getPosition().y;

	camera->setCamPosition(camPos);
	camera->setCamOrientation(camOri);
}

float charRot = 0;

void Game::input()
{
	float speed = 100;

	glm::vec3 lastPos = entities[0]->getTransform()->getPosition();

	if (Input::keyStates['w'])
	{
		glm::vec3 nextPos = glm::vec3(entities[0]->getTransform()->getPosition().x,
			entities[0]->getTransform()->getPosition().y + (float)Display::frameTime * speed,
			entities[0]->getTransform()->getPosition().z);
		if (!MapCoordinate::isOfCollisionType(mapM->getMapCoordinateForPlayerMovement(nextPos).terrain))
			entities[0]->getTransform()->translate(nextPos.x, nextPos.y, nextPos.z);
		else
		{
			if (!Input::keyStates['s'] && !Input::keyStates['a'] && !Input::keyStates['d'])
			{
				for (int i = 1; i <= SLIDE_FACTOR; i++)
				{
					glm::vec3 leftPos = nextPos;
					leftPos.x = leftPos.x - (float)i * ((float)Display::frameTime * speed/10);
					glm::vec3 rightPos = nextPos;
					rightPos.x = rightPos.x + (float)i * ((float)Display::frameTime * speed / 10);
					if (!MapCoordinate::isOfCollisionType(mapM->getMapCoordinateForPlayerMovement(rightPos).terrain))
					{
						glm::vec3 inlineRightPos = lastPos;
						inlineRightPos.x = inlineRightPos.x + SLIDE_SPEED_FACTOR;
						entities[0]->getTransform()->translate(inlineRightPos.x, inlineRightPos.y, inlineRightPos.z);
						break;
					}
					else
					if (!MapCoordinate::isOfCollisionType(mapM->getMapCoordinateForPlayerMovement(leftPos).terrain))
					{
						glm::vec3 inlineLeftPos = lastPos;
						inlineLeftPos.x = inlineLeftPos.x - SLIDE_SPEED_FACTOR;
						entities[0]->getTransform()->translate(inlineLeftPos.x, inlineLeftPos.y, inlineLeftPos.z);
						break;
					}
				}
			}
		}
	}

	if (Input::keyStates['s'])
	{
		glm::vec3 nextPos = glm::vec3(entities[0]->getTransform()->getPosition().x,
			entities[0]->getTransform()->getPosition().y - (float)Display::frameTime * speed,
			entities[0]->getTransform()->getPosition().z);
		if (!MapCoordinate::isOfCollisionType(mapM->getMapCoordinateForPlayerMovement(nextPos).terrain))
			entities[0]->getTransform()->translate(nextPos.x, nextPos.y, nextPos.z);
		else
		{
			if (!Input::keyStates['w'] && !Input::keyStates['a'] && !Input::keyStates['d'])
			{
				for (int i = 1; i <= SLIDE_FACTOR; i++)
				{
					glm::vec3 leftPos = nextPos;
					leftPos.x = leftPos.x - (float)i * ((float)Display::frameTime * speed / 10);
					glm::vec3 rightPos = nextPos;
					rightPos.x = rightPos.x + (float)i * ((float)Display::frameTime * speed / 10);
					if (!MapCoordinate::isOfCollisionType(mapM->getMapCoordinateForPlayerMovement(rightPos).terrain))
					{
						glm::vec3 inlineRightPos = lastPos;
						inlineRightPos.x = inlineRightPos.x + SLIDE_SPEED_FACTOR;
						entities[0]->getTransform()->translate(inlineRightPos.x, inlineRightPos.y, inlineRightPos.z);
						break;
					}
					else
						if (!MapCoordinate::isOfCollisionType(mapM->getMapCoordinateForPlayerMovement(leftPos).terrain))
						{
							glm::vec3 inlineLeftPos = lastPos;
							inlineLeftPos.x = inlineLeftPos.x - SLIDE_SPEED_FACTOR;
							entities[0]->getTransform()->translate(inlineLeftPos.x, inlineLeftPos.y, inlineLeftPos.z);
							break;
						}
				}
			}
		}
	}

	if (Input::keyStates['a'])
	{
		glm::vec3 nextPos = glm::vec3(entities[0]->getTransform()->getPosition().x - (float)Display::frameTime * speed,
			entities[0]->getTransform()->getPosition().y,
			entities[0]->getTransform()->getPosition().z);
		if (!MapCoordinate::isOfCollisionType(mapM->getMapCoordinateForPlayerMovement(nextPos).terrain))
			entities[0]->getTransform()->translate(nextPos.x, nextPos.y, nextPos.z);
		else
		{
			if (!Input::keyStates['w'] && !Input::keyStates['s'] && !Input::keyStates['d'])
			{
				for (int i = 1; i <= SLIDE_FACTOR; i++)
				{
					glm::vec3 southPos = nextPos;
					southPos.y = southPos.y - (float)i * ((float)Display::frameTime * speed / 10);
					glm::vec3 northPos = nextPos;
					northPos.y = northPos.y + (float)i * ((float)Display::frameTime * speed / 10);
					if (!MapCoordinate::isOfCollisionType(mapM->getMapCoordinateForPlayerMovement(southPos).terrain))
					{
						glm::vec3 inlineSouthPos = lastPos;
						inlineSouthPos.y = inlineSouthPos.y - SLIDE_SPEED_FACTOR;
						entities[0]->getTransform()->translate(inlineSouthPos.x, inlineSouthPos.y, inlineSouthPos.z);
						break;
					}
					else
						if (!MapCoordinate::isOfCollisionType(mapM->getMapCoordinateForPlayerMovement(northPos).terrain))
						{
							glm::vec3 inlineNorthPos = lastPos;
							inlineNorthPos.y = inlineNorthPos.y + SLIDE_SPEED_FACTOR;
							entities[0]->getTransform()->translate(inlineNorthPos.x, inlineNorthPos.y, inlineNorthPos.z);
							break;
						}
				}
			}
		}
	}

	if (Input::keyStates['d'])
	{
		glm::vec3 nextPos = glm::vec3(entities[0]->getTransform()->getPosition().x + (float)Display::frameTime * speed,
			entities[0]->getTransform()->getPosition().y,
			entities[0]->getTransform()->getPosition().z);
		if (!MapCoordinate::isOfCollisionType(mapM->getMapCoordinateForPlayerMovement(nextPos).terrain))
			entities[0]->getTransform()->translate(nextPos.x, nextPos.y, nextPos.z);
		else
		{
			if (!Input::keyStates['w'] && !Input::keyStates['s'] && !Input::keyStates['a'])
			{
				for (int i = 1; i <= SLIDE_FACTOR; i++)
				{
					glm::vec3 southPos = nextPos;
					southPos.y = southPos.y - (float)i * ((float)Display::frameTime * speed / 10);
					glm::vec3 northPos = nextPos;
					northPos.y = northPos.y + (float)i * ((float)Display::frameTime * speed / 10);
					if (!MapCoordinate::isOfCollisionType(mapM->getMapCoordinateForPlayerMovement(southPos).terrain))
					{
						glm::vec3 inlineSouthPos = lastPos;
						inlineSouthPos.y = inlineSouthPos.y - SLIDE_SPEED_FACTOR;
						entities[0]->getTransform()->translate(inlineSouthPos.x, inlineSouthPos.y, inlineSouthPos.z);
						break;
					}
					else
						if (!MapCoordinate::isOfCollisionType(mapM->getMapCoordinateForPlayerMovement(northPos).terrain))
						{
							glm::vec3 inlineNorthPos = lastPos;
							inlineNorthPos.y = inlineNorthPos.y + SLIDE_SPEED_FACTOR;
							entities[0]->getTransform()->translate(inlineNorthPos.x, inlineNorthPos.y, inlineNorthPos.z);
							break;
						}
				}
			}
		}
	}

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
		charRot += 0.5f;
		entities[0]->getTransform()->rotate(charRot, glm::vec3(1, 0, 0));
	}

	if (Input::keyStates['l'])
	{
	if (Mesh::drawForm == GL_TRIANGLES)
		Mesh::drawForm = GL_LINES;
	else
		Mesh::drawForm = GL_TRIANGLES;
	}
}
