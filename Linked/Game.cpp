#include "Game.h"
#include "MapObjectImageLoader.h"
#include <string>
#include <iostream>
#include "Mesh.h"
#include "Primitive.h"
#include "PrimitiveShader.h"
#include "Input.h"
#include "Entity.h"

/*Game::Game()
{
	//std::string a = "./res/imagem_hoshoyo.png";
	//std::string enumr = "teste";
	//MapObjectImageLoader *imagem = new MapObjectImageLoader(a, 3);
	//MapObject obj = imagem->getMapCoordinate(glm::vec3(0, 0, 0));
	test(0, 0);
	test(1, 0);
	test(0, 1);
	test(1, 1);
}*/

void Game::test(int x, int y)
{
	std::string a = "./res/imagem_hoshoyo.png";
	std::string enumr = "teste";
	MapObjectImageLoader *imagem = new MapObjectImageLoader(a, 3);
	MapObject obj = imagem->getMapCoordinate(glm::vec3(x, y, 0));
	switch (obj)
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

Mesh* mesh;
PrimitiveShader* shader;
Entity* entity;

Game::Game()
{
	std::string shaderPath = "./shaders/normalshader";
	mesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 0.3f, 0.3f), new Texture("./res/Textures/predio.jpg"));
	entity = new Entity(new Transform(), mesh);
	shader = new PrimitiveShader(shaderPath);
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
}
