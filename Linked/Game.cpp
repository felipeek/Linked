// Project Libs
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
#include "MonsterFactory.h"
#include "GameEntityFactory.h"
#include "Projectile.h"
#include "Player.h"

#include "Camera.h"
#include "PrimitiveShader.h"
#include "MapShader.h"
#include "GUIShader.h"
#include "CommonShader.h"
#include "TextShader.h"
#include "Light.h"

#include "RangeAttack.h"

#include "GUI.h"
#include "Text.h"
#include "Chat.h"
#include "HPBar.h"
#include "ZurikiRageSkill.h"
#include "HoshoyoExplosionSkill.h"

#include "network\Packet.h"
#include "PacketController.h"

#include "FrameBuffer.h"

// Standard libs
#include <iostream>
#include <string>

FrameBuffer* fb;

Game::Game(int windowWidth, int windowHeight)
{	
	double lastT = 0;
	this->frameBuffer = new FrameBuffer(windowWidth, windowHeight);
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	PacketController::game = this;
	lastT = Time::getTime();
	this->createGraphicElements(windowWidth, windowHeight);
	std::cout <<"createGraphicElements "<< Time::getTime() - lastT << std::endl;
	lastT = Time::getTime();
	this->createMap();
	std::cout <<"createMap "<< Time::getTime() - lastT << std::endl;
	lastT = Time::getTime();
#ifdef SINGLEPLAYER
	this->createOfflinePlayer();
	std::cout << "createOfflinePlayer " << Time::getTime() - lastT << std::endl;
#endif
#ifdef MULTIPLAYER
	this->createUDPConnection();
	this->waitForCreationOfOnlinePlayer();
	
#endif
	lastT = Time::getTime();
	this->createGUI();
	std::cout << "createGUI " << Time::getTime() - lastT << std::endl;
#ifdef MULTIPLAYER
	this->loadMonstersAndEntities(false, true);
#endif

#ifdef SINGLEPLAYER
	lastT = Time::getTime();
	this->loadMonstersAndEntities(true, true);
	std::cout << "loadMonstersAndEntities " << Time::getTime() - lastT << std::endl;
#endif
	fb = new FrameBuffer(2048, 2048, true);
	//lastTime = 0;
}

Game::~Game()
{
	if (camera != nullptr) delete camera;
	if (light != nullptr) delete light;
	if (primitiveShader != nullptr) delete primitiveShader;
	if (projectileShader != nullptr) delete projectileShader;
	if (mapShader != nullptr) delete mapShader;
	if (monsterFactory != nullptr) delete monsterFactory;
	if (gameEntityFactory != nullptr) delete gameEntityFactory;
	if (this->map != nullptr) delete map;
	if (this->gui != nullptr) delete gui;
	if (this->water != nullptr) delete water;

	// delete vectors content
	for (Monster* monster : monsters)
		delete monster;
	for (Entity* entity : entities)
		delete entity;
	for (GameEntity* entity : gameEntities)
		delete entity;

#ifdef SINGLEPLAYER
	if (this->localPlayer != nullptr)	delete localPlayer;
#endif
#ifdef MULTIPLAYER
	udpClient->virtualDisconnection();
	if (this->udpClient != nullptr) delete udpClient;
#endif
}

void Game::createGraphicElements(int windowsWidth, int windowsHeight)
{
	// Camera
	this->camera = new Camera(glm::vec3(0, 0, 50), glm::vec3(0, 0, 0), 70.0f, (float)windowsWidth / windowsHeight, 0.1f, 1000.0f);
	Input::mouseAttack.setCamera(this->camera);

	// Light
	this->light = new Light(glm::vec3(100, 500, 50), glm::vec3(1, 0.95f, 0.8f));

	// Shaders
	this->primitiveShader = new PrimitiveShader("./shaders/normalshader", camera, light);
	this->commonShader = new CommonShader("./shaders/commonshader", camera, light);
	this->projectileShader = new CommonShader("./shaders/projectile", camera, light);
	this->mapShader = new MapShader("./shaders/mapshader_shadow", camera, light);
}

void Game::createMap()
{
	std::string mapPath = TERRAIN_MAP_PATH;
	std::string entitiesMapPath = ENTITIES_MAP_PATH;
	std::string monsterMapPath = MONSTER_MAP_PATH;

	this->monsterFactory = new MonsterFactory();
	this->gameEntityFactory = new GameEntityFactory();

	this->map = new Map(mapPath, entitiesMapPath, monsterMapPath, 3, monsterFactory, gameEntityFactory);
}

void Game::createOfflinePlayer()
{
#ifdef SINGLEPLAYER
	// TODO delete player Mesh, Transform, Texture
	Mesh* playerMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f, 12, 0));
	this->localPlayer = new Player(new Transform(glm::vec3(440, 500, 1.5f), 45, glm::vec3(1, 0, 0), glm::vec3(2, 2, 2)), playerMesh, new Texture("./res/Monsters/Sprites/greenwarrior.png"), &monsters, map);
	this->localPlayer->setHp(100);
	this->localPlayer->setName("JaOwnes");
	PacketController::localPlayer = localPlayer;

	this->localPlayer->setSpeedBasis(26);
	this->localPlayer->setMaximumHpBasis(100);
	this->localPlayer->setDefenseBasis(100);
	this->localPlayer->setMagicalPowerBasis(20);
	// TODO delete skill1, 2, 3, 4
	Skill* skill1 = new HoshoyoExplosionSkill(&monsters);
	skill1->setSlot(SLOT_1);
	localPlayer->addNewSkill(skill1);
	
	Skill* skill2 = new ZurikiRageSkill(&monsters);
	skill2->setSlot(SLOT_2);
	localPlayer->addNewSkill(skill2);
	
	Skill* skill3 = new HoshoyoExplosionSkill(&monsters);
	skill3->setSlot(SLOT_3);
	localPlayer->addNewSkill(skill3);
	
	Skill* skill4 = new HoshoyoExplosionSkill(&monsters);
	skill4->setSlot(SLOT_4);
	localPlayer->addNewSkill(skill4);
#endif
}

void Game::waitForCreationOfOnlinePlayer()
{
	do
	{
		this->udpClient->receivePackets();
		Sleep(100);
	}
	while (this->localPlayer == nullptr);

	this->localPlayer = PacketController::localPlayer;
}

void Game::createOnlinePlayer(short* data, bool isLocalPlayer)
{
#ifdef MULTIPLAYER
	glm::vec3 localPlayerPosition = glm::vec3(data[8], data[9], data[10]);
	// TODO: delete player Mesh
	Mesh* playerMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f, 12, 0));
	// TODO: delete designedPlayer, and his transform/texture
	Player* designedPlayer;

	if (data[0] == 0)
	{
		designedPlayer = new Player(new Transform(localPlayerPosition, 45, glm::vec3(1, 0, 0), glm::vec3(2, 2, 2)), playerMesh, new Texture("./res/Monsters/Sprites/greenwarrior.png"), &monsters, map);
		designedPlayer->setName("JaOwnes");
	}
	else if (data[0] == 1)
	{
		designedPlayer = new Player(new Transform(localPlayerPosition, 45, glm::vec3(1, 0, 0), glm::vec3(2, 2, 2)), playerMesh, new Texture("./res/Monsters/Sprites/greenwarrior.png"), &monsters, map);
		designedPlayer->setName("HoEnchant");
	}

	designedPlayer->setTotalMaximumHp(data[1]);
	designedPlayer->setHp(data[2]);
	designedPlayer->setTotalAttack(data[3]);
	designedPlayer->setTotalDefense(data[4]);
	designedPlayer->setTotalMagicalPower(data[5]);
	designedPlayer->setTotalSpeed(data[6]);
	designedPlayer->setTotalAttackSpeed(data[7]);

	if (isLocalPlayer)
	{
		this->localPlayer = designedPlayer;
		PacketController::localPlayer = this->localPlayer;
	}
	else
	{
		this->secondPlayer = designedPlayer;
		PacketController::secondPlayer = this->secondPlayer;
	}
#endif
}

void Game::createGUI()
{
	this->gui = new GUI(localPlayer, "./shaders/textshader", "./shaders/fontshader", "./fonts/consola.ttf");
	PacketController::gui = this->gui;
}

void Game::loadMonstersAndEntities(bool loadMonsters, bool loadEntities)
{
	double lastT = 0;
	lastT = Time::getTime();
	Mesh* mapMesh = new Mesh(new Grid(MAP_SIZE, this->map));
	std::cout << "Create Grid:" << Time::getTime() - lastT << std::endl;

	// TODO: delete EntityMap transform and textures
	lastT = Time::getTime();
	this->entityMap = new EntityMap(new Transform(), mapMesh,
		new Texture("./res/Maps/stonePath.png"),
		new Texture("./res/Maps/mountain.jpg"),
		new Texture("./res/Maps/water.jpg"),
		new Texture("./res/Maps/grassTex.png"),
		new Texture(TERRAIN_MAP_PATH));

	// TODO: delete waterMesh Mesh, texture and transform
	Mesh* waterMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), (float)MAP_SIZE, (float)MAP_SIZE));
	Texture* waterTexture = new Texture("./res/Maps/water.jpg");
	waterTexture->setTileAmount(100);
	water = new Entity(new Transform(glm::vec3(0,0,-0.3f)), waterMesh, waterTexture);
	std::cout << "load rest of map: " << Time::getTime() - lastT << std::endl;

	lastT = Time::getTime();
	// Load monsters and entities
	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			MapCoordinate coordinate = map->getMapCoordinateForMapCreation(glm::vec3(i, j, 0));
			Monster *monster = coordinate.mapMonster.monster;
			GameEntity *gameEntity = coordinate.mapGameEntity.gameEntity;

			if (loadMonsters)
			{
				if (coordinate.mapMonster.monsterExists)
				{
					if (!map->coordinateHasCollision(glm::vec3(i, j, 0)))
					{
						monster->getTransform()->translate((float)i, (float)j, 1.3f);
						monsters.push_back(monster);
					}
					else
						delete monster;
				}
			}
			
			if (loadEntities)
			{
				if (coordinate.mapGameEntity.gameEntityExists)
				{
					gameEntity->getTransform()->translate((float)i, (float)j, 0);
					gameEntities.push_back(gameEntity);
				}
			}
		}
	}
	std::cout << "Create entities and monsters" << Time::getTime() - lastT << std::endl;
}

void Game::createUDPConnection()
{
	udpClient = new UDPClient(SERVER_PORT, SERVER_IP);
	PacketController::udpClient = udpClient;
	udpClient->virtualConnection();
}

void Game::createMonster(int monsterId, short* data)
{
#ifdef MULTIPLAYER
	// TODO: delete newMonster
	int monsterHp = data[0];
	glm::vec3 monsterRgb = glm::vec3(data[1], data[2], data[3]);
	glm::vec3 monsterPosition = glm::vec3(data[4], data[5], data[6]);
	Monster* newMonster = this->monsterFactory->getMonsterOfMapColor(monsterRgb);
	newMonster->getTransform()->translate(monsterPosition.x, monsterPosition.y, monsterPosition.z);
	newMonster->setId(monsterId);
	newMonster->setHp(monsterHp);

	if (!this->map->coordinateHasCollision(monsterPosition))
		this->monsters.push_back(newMonster);
#endif
}

Monster* Game::getMonsterOfId(int id)
{
#ifdef MULTIPLAYER
	for (unsigned int i = 0; i < monsters.size(); i++)
		if (monsters[i]->getId() == id)
			return monsters[i];
#endif

	return nullptr;
}

void Game::render()
{
	// Map
	glm::mat4 oldProj = camera->getProjection();
	glm::vec3 camOldPos = camera->getPosition();
	for (int i = 0; i < 2; i++){
		if (i == 0)
		{
			glm::vec3 lightPos = light->lightPosition;

			glm::mat4 ort = glm::ortho(-55.0f, 55.0f, -55.0f, 55.0f, 0.1f, 150.0f);
			//glm::mat4 ort = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 0.1f, 150.0f);
			camera->setProjectionMatrix(ort);
			camera->setCamPosition(lightPos);

			glm::mat4 lightSpace = ort * camera->getView();
			mapShader->useShader();
			glUniformMatrix4fv(glGetUniformLocation(mapShader->getShader(), "lightSpaceMatrix"), 1, GL_FALSE, &lightSpace[0][0]);
			glBindTexture(GL_TEXTURE_2D, fb->textureID);
			glActiveTexture(GL_TEXTURE5);
			glUniform1i(glGetUniformLocation(mapShader->getShader(), "shadowMap"), 5);
			mapShader->stopShader();
			fb->renderDepth();
		}
		else
		{
			camera->setProjectionMatrix(oldProj);
			camera->setCamPosition(camOldPos);
			fb->normalRender();
		}

		entityMap->render(mapShader);
		water->render(commonShader);


		// Generic entities (Player only at the moment)
		for (Entity* e : entities)
		{
			try{
				e->render(primitiveShader);
			}
			catch (...){
				std::cerr << "Error rendering entity" << std::endl;
			}
		}
		// Monsters
		for (Entity* e : monsters)
		{
			try{
				e->render(primitiveShader);
			}
			catch (...){
				std::cerr << "Error rendering entity" << std::endl;
			}
		}

		// Common static entities
		for (Entity* e : gameEntities)
		{
			try{
				e->render(commonShader);
			}
			catch (...){
				std::cerr << "Error rendering entity" << std::endl;
			}
		}

		// Player
		localPlayer->render(primitiveShader, gui->getTextRenderer(), projectileShader);

		// Second Player
#ifdef MULTIPLAYER
		if (secondPlayer != NULL)
			secondPlayer->render(primitiveShader, gui->getTextRenderer(), projectileShader);
#endif MULTIPLAYER
	}
	// Render GUI (Order is important)
	gui->render();

}

void Game::update()
{
#ifdef MULTIPLAYER
	udpClient->receivePackets();
	if (Chat::msg != "")
	{
		if (Chat::msg.compare("/where") == 0)
		{
			std::stringstream ss;
			ss << localPlayer->getTransform()->getPosition().x << ", " << localPlayer->getTransform()->getPosition().y << ", " << localPlayer->getTransform()->getPosition().z;
			gui->setNextMessage(ss.str());
			Chat::msg = "";
		}
		else{
			gui->setNextMessage(Chat::appendPlayerName(localPlayer->getName()));
			//gui->setNextMessage(Chat::msg);
			udpClient->sendPackets(Packet(Chat::msg, -1));
			Chat::msg = "";
		}
	}
#endif
#ifdef SINGLEPLAYER
	if (Chat::msg != "")
	{
		gui->setNextMessage(Chat::msg);
		Chat::msg = "";
	}
#endif
	// Camera update
	camera->update(localPlayer->getTransform()->getPosition());

	// Light update
	glm::vec3 playerPos = localPlayer->getTransform()->getPosition();
	playerPos.x -= 30.0f;
	playerPos.y -= 30.0f;
	light->update(playerPos);

	// Player update	
	localPlayer->update(this->map);

#ifdef MULTIPLAYER
	if (secondPlayer != NULL) 
		secondPlayer->update(this->map);
#endif
	
	// Monsters update
	for (unsigned int i = 0; i < monsters.size(); i++)
		monsters[i]->update(map, localPlayer);

	for (unsigned int i = 0; i < monsters.size(); i++)
		if (!monsters[i]->isOnScreen())
		{
			delete monsters[i];
			monsters.erase(monsters.begin() + i);
		}

	// GUI update
	gui->update();
}

void Game::input()
{	
	if (!Chat::isChatActive())
	{
		Input::mouseAttack.update();
		camera->input();
		light->input();
		localPlayer->input(this->map);
	}

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
			} while (map->getMapCoordinateForMapCreation(newPos).terrain != NORMAL_FLOOR);

			lastTime = Time::getTime();
			localPlayer->getTransform()->translate(newPos.x, newPos.y, localPlayer->getTransform()->getPosition().z);
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