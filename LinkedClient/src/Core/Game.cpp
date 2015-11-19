// Project Libs
#include "Game.h"
#include "Display.h"
#include "LinkedTime.h"
#include "Input.h"

#include "Map.h"
#include "MapTerrain.h"
#include "MapEntity.h"

#include "Mesh.h"
#include "Grid.h"
#include "EntityMap.h"
#include "MonsterFactory.h"
#include "GameEntityFactory.h"
#include "Monster.h"
#include "GameEntity.h"
#include "Projectile.h"
#include "Player.h"

#include "Cursor.h"
#include "Camera.h"
#include "PrimitiveShader.h"
#include "MapShader.h"
#include "GUIShader.h"
#include "CommonShader.h"
#include "TextShader.h"
#include "SkillShader.h"
#include "Light.h"

#include "RangeAttack.h"
#include "Projectile.h"

#include "GUI.h"
#include "Chat.h"
#include "HPBar.h"
#include "HoshoyoExplosionSkill.h"
#include "CureBlessingSkill.h"
#include "SwapSkill.h"
#include "LinkSkill.h"

#include "Packet.h"
#include "PacketController.h"
#include "UDPClient.h"

#include "FrameBuffer.h"

// Standard libs
#include <iostream>
#include <string>

bool Game::multiplayer = false;
int Game::server_port = 9090;

std::string Game::server_ip = "127.0.0.1";
//std::string Game::server_ip = "201.21.40.57";
//std::string Game::server_ip = "hoshoyo.servegame.com";

Cursor* Game::cursor = nullptr;

Game::Game(int windowWidth, int windowHeight)
	: windowWidth(windowWidth), windowHeight(windowHeight)
{	
	PacketController::game = this;

	this->createGraphicElements(windowWidth, windowHeight);
	this->createMap();
	
	if (Game::multiplayer)
	{
		this->createUDPConnection();
		PacketController::onlinePlayers = &this->onlinePlayers;
		this->waitForCreationOfOnlinePlayer();
		this->loadMonstersAndEntities(false, true);
	}
	else
	{
		this->createOfflinePlayer();
		this->loadMonstersAndEntities(true, true);
	}
	this->createGUI();
}

Game::~Game()
{
	if (cursor != nullptr) delete cursor;
	if (camera != nullptr) delete camera;
	if (light != nullptr) delete light;
	if (primitiveShader != nullptr) delete primitiveShader;
	if (projectileShader != nullptr) delete projectileShader;
	if (mapShader != nullptr) delete mapShader;
	if (commonShader != nullptr) delete commonShader;
	if (skillShader != nullptr) delete skillShader;
	if (monsterFactory != nullptr) delete monsterFactory;
	if (gameEntityFactory != nullptr) delete gameEntityFactory;
	if (worldSkillShader != nullptr) delete worldSkillShader;
	if (this->map != nullptr) delete map;
	if (this->gui != nullptr) delete gui;
	if (this->water != nullptr) delete water;
	if (this->entityMap != nullptr) delete entityMap;

	if (this->frameBuffer != nullptr) delete frameBuffer;

	std::cout << "MONSTER STARTED" << std::endl;
	std::cout << "-----------------" << std::endl;
	// delete vectors content
	for (Monster* monster : monsters)
		delete monster;
		
	std::cout << "GENERAL ENTITIES STARTED" << std::endl;
	std::cout << "-----------------" << std::endl;
	for (Entity* entity : entities)
		delete entity;
	
	std::cout << "GAMEENTITIES STARTED" << std::endl;
	std::cout << "-----------------" << std::endl;
	for (GameEntity* entity : gameEntities)
		delete entity;

	std::cout << "rest" << std::endl;
	std::cout << "-----------------" << std::endl;
	if (this->localPlayer != nullptr) delete localPlayer;

	if (Game::multiplayer)
	{
		udpClient->virtualDisconnection();
		if (this->udpClient != nullptr) delete udpClient;
	}
}

void Game::createGraphicElements(int windowWidth, int windowHeight)
{
	// Cursor
	Game::cursor = new Cursor();

	// Camera
	this->camera = new Camera(glm::vec3(0, 0, 50), glm::vec3(0, 0, 0), 70.0f, (float)windowWidth / windowHeight, 0.1f, 1000.0f);
	Input::mouseAttack.setCamera(this->camera);

	// Light
	this->light = new Light(glm::vec3(100, 500, 50), glm::vec3(1, 0.95f, 0.8f));

	// Shaders
	this->primitiveShader = new PrimitiveShader("./shaders/normalshader", camera, light);
	this->commonShader = new CommonShader("./shaders/commonshader", camera, light);
	this->projectileShader = new CommonShader("./shaders/projectile", camera, light);
	this->mapShader = new MapShader("./shaders/mapshader_shadow", camera, light);
	this->skillShader = new SkillShader("./shaders/fontshader");
	this->worldSkillShader = new PrimitiveShader("./shaders/skillshader", camera, light);

	// Shadows
	frameBuffer = new FrameBuffer(SHADOW_BUFFER_SIZE, SHADOW_BUFFER_SIZE);
	frameBuffer->genShadowMap(glm::vec4(1,1,1,1));
	frameBuffer->genLightCamera(55.0f);
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
	Mesh* playerMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f, 12, 0));
	this->localPlayer = new Player(new Transform(glm::vec3(440, 500, PLAYER_HEIGHT), 45, glm::vec3(1, 0, 0), glm::vec3(2, 2, 2)), playerMesh, new Texture("./res/Monsters/Sprites/greenwarrior.png"), &monsters, map);
	this->localPlayer->setHp(100);
	this->localPlayer->setName("JaOwnes");
	this->localPlayer->setClientId(0);
	PacketController::localPlayer = localPlayer;

	this->localPlayer->setSpeedBasis(26);
	this->localPlayer->setMaximumHpBasis(100);
	this->localPlayer->setDefenseBasis(100);
	this->localPlayer->setMagicalPowerBasis(20);

	Skill* skill1 = new LinkSkill(PLAYER, &monsters, &onlinePlayers, &localPlayer);
	skill1->setSlot(SLOT_1);
	localPlayer->addNewSkill(skill1);
	
	Skill* skill2 = new SwapSkill(PLAYER, &monsters, &onlinePlayers, &localPlayer);
	skill2->setSlot(SLOT_2);
	localPlayer->addNewSkill(skill2);
	
	Skill* skill3 = new CureBlessingSkill(PLAYER, &monsters, &onlinePlayers, &localPlayer);
	skill3->setSlot(SLOT_3);
	localPlayer->addNewSkill(skill3);
	
	Skill* skill4 = new HoshoyoExplosionSkill(PLAYER, &monsters, &onlinePlayers, &localPlayer);
	skill4->setSlot(SLOT_4);
	localPlayer->addNewSkill(skill4);
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
	glm::vec3 localPlayerPosition = glm::vec3(data[8], data[9], PLAYER_HEIGHT);
	// TODO: delete player Mesh
	Mesh* playerMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f, 12, 0));

	// TODO: delete designedPlayer, and his transform/texture

	Player* designedPlayer = new Player(new Transform(localPlayerPosition, 45, glm::vec3(1, 0, 0), glm::vec3(2, 2, 2)), playerMesh, new Texture("./res/Monsters/Sprites/greenwarrior.png"), &monsters, map);
	
	designedPlayer->setName("new player");
	designedPlayer->setClientId(data[0]);

	designedPlayer->setTotalMaximumHp(data[1]);
	designedPlayer->setHp(data[2]);
	designedPlayer->setTotalAttack(data[3]);
	designedPlayer->setTotalDefense(data[4]);
	designedPlayer->setTotalMagicalPower(data[5]);
	designedPlayer->setTotalSpeed(data[6]);
	designedPlayer->setTotalAttackSpeed(data[7]);

	/* FOR NOW, SKILLS MUST BE THE SAME AS THEY ARE DEFINED SERVER-SIDE (SAME SLOTS, ALSO) */
	Skill* skill1 = new LinkSkill(PLAYER, &monsters, &onlinePlayers, &localPlayer);
	skill1->setSlot(SLOT_1);
	designedPlayer->addNewSkill(skill1);

	Skill* skill2 = new SwapSkill(PLAYER, &monsters, &onlinePlayers, &localPlayer);
	skill2->setSlot(SLOT_2);
	designedPlayer->addNewSkill(skill2);

	Skill* skill3 = new CureBlessingSkill(PLAYER, &monsters, &onlinePlayers, &localPlayer);
	skill3->setSlot(SLOT_3);
	designedPlayer->addNewSkill(skill3);

	Skill* skill4 = new HoshoyoExplosionSkill(PLAYER,&monsters, &onlinePlayers, &localPlayer);
	skill4->setSlot(SLOT_4);
	designedPlayer->addNewSkill(skill4);

	if (isLocalPlayer)
	{
		this->localPlayer = designedPlayer;
		PacketController::localPlayer = this->localPlayer;
		designedPlayer->setType(LOCAL);
	}
	else
	{
		this->onlinePlayers.push_back(designedPlayer);
		designedPlayer->setType(NETWORK);
	}
}

void Game::createGUI()
{
	this->gui = new GUI(localPlayer);
	for (Skill* s : this->localPlayer->getSkills())
		this->gui->addSkillIcon(s->getSkillIcon());
	PacketController::gui = this->gui;
	Chat::gui = this->gui;	
}

void Game::loadMonstersAndEntities(bool loadMonsters, bool loadEntities)
{
	Mesh* mapMesh = new Mesh(new Grid(MAP_SIZE, this->map));

	// TODO: delete EntityMap transform and textures
	this->entityMap = new EntityMap(new Transform(), mapMesh,
		new Texture("./res/Maps/stonePath.png"),
		new Texture("./res/Maps/mountain.jpg"),
		new Texture("./res/Maps/water.jpg"),
		new Texture("./res/Maps/grassTex.png"),
		new Texture(TERRAIN_MAP_PATH));
	entityMap->setShadowTexture(frameBuffer->getTexture());

	// TODO: delete waterMesh Mesh, texture and transform
	Mesh* waterMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), (float)MAP_SIZE, (float)MAP_SIZE));
	Texture* waterTexture = new Texture("./res/Maps/water.jpg");
	waterTexture->setTileAmount(100);
	water = new Entity(new Transform(glm::vec3(0,0,-0.3f)), waterMesh, waterTexture);


	// TODO: verify allocation in a loop ( if causes performance overhead )
	//monsters.resize(77);
	//gameEntities.resize(30);
	
	// Load monsters and entities
	for (int i = 0/*, m=0, e=0*/; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			MapCoordinate coordinate = map->getMapCoordinateForMapCreation(glm::vec3(i, j, 0));
			Monster *monster = coordinate.mapMonster.monster;
			GameEntity *gameEntity = coordinate.mapGameEntity.gameEntity;
			
			if (coordinate.mapMonster.monsterExists)
			{
				if (!map->coordinateHasCollision(glm::vec3(i, j, 0)) && loadMonsters/* && m < 1*/)
				{
					monster->getTransform()->translate((float)i, (float)j, 1.3f);
					monsters.push_back(monster);
					//m++;
				}
				else
				{
					delete monster;
				}
			}
			
			if (coordinate.mapGameEntity.gameEntityExists && loadEntities)
			{
				gameEntity->getTransform()->translate((float)i, (float)j, 0);
				gameEntities.push_back(gameEntity);
			}
		}
	}
}

void Game::createUDPConnection()
{
	udpClient = new UDPClient(Game::server_port, Game::server_ip);
	PacketController::udpClient = udpClient;
	udpClient->virtualConnection();
}

void Game::createMonster(short* data)
{
	// TODO: delete newMonster

	// do not tests collision intentionally
	int monsterId = data[0];
	int monsterHp = data[1];
	glm::vec3 monsterRgb = glm::vec3(data[2], data[3], data[4]);
	glm::vec3 monsterPosition = glm::vec3(data[5], data[6], data[7]);

	Monster* newMonster = this->monsterFactory->getMonsterOfMapColor(monsterRgb);
	newMonster->getTransform()->translate(monsterPosition.x, monsterPosition.y, monsterPosition.z);
	newMonster->setId(monsterId);
	newMonster->setHp(monsterHp);
	this->monsters.push_back(newMonster);
}

Monster* Game::getMonsterOfId(int id)
{
	for (unsigned int i = 0; i < monsters.size(); i++)
		if (monsters[i]->getId() == id)
			return monsters[i];

	return nullptr;
}

void Game::destroyProjectileOfId(int id)
{
	for (unsigned int i = 0; i < this->onlinePlayers.size(); i++)
		for (unsigned int j = 0; j < this->onlinePlayers[i]->getRangeAttack()->getAttacks()->size(); j++)
			if ((*this->onlinePlayers[i]->getRangeAttack()->getAttacks())[j]->getId() == id)
			{
				std::vector<Projectile*>* attacks = this->onlinePlayers[i]->getRangeAttack()->getAttacks();
				delete (*attacks)[j];
				attacks->erase((*attacks).begin() + j);
				break;
			}
}

void Game::render()
{
	/* FIRST PASS (SHADOW PASS) */
	renderFirstPass();

	/* SECOND PASS (COLOR PASS) */
	renderSecondsPass();
}

void Game::renderFirstPass()
{	
	// Set Camera on shaders
	mapShader->setCamera(frameBuffer->getCamera());
	commonShader->setCamera(frameBuffer->getCamera());
	primitiveShader->setCamera(frameBuffer->getCamera());
	projectileShader->setCamera(frameBuffer->getCamera());

	// Prime render
	frameBuffer->renderDepth();

	// Map
	//entityMap->render(mapShader);
	((Entity*)entityMap)->render(commonShader);
	water->render(commonShader);

	// Monsters
	for (Monster* m : monsters)
	{
		try{
			if (Game::multiplayer)
			{
				if (!localPlayer->isFogOfWar(m->getTransform()->getPosition()) && !m->shouldTranslate())
					m->render(primitiveShader);
			}
			else
				m->render(primitiveShader);
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
	localPlayer->hpBarRenderOptions(false);
	localPlayer->render(primitiveShader, skillShader, worldSkillShader, nullptr, projectileShader);

	// Second Player
	if (Game::multiplayer)
	{
		for (Player* player : this->onlinePlayers)
		{
			player->hpBarRenderOptions(false);
			player->render(primitiveShader, skillShader, worldSkillShader, nullptr, projectileShader);
		}
	}
}

void Game::renderSecondsPass()
{
	// Set Camera on shaders
	mapShader->setCamera(camera);
	commonShader->setCamera(camera);
	primitiveShader->setCamera(camera);
	projectileShader->setCamera(camera);

	// Prime render
	frameBuffer->normalRender(windowWidth, windowHeight);
	
	// Map
	entityMap->render(mapShader, frameBuffer->getCamera());
	water->render(commonShader);

	// Player
	localPlayer->hpBarRenderOptions(true);
	localPlayer->render(primitiveShader, skillShader, worldSkillShader, nullptr, projectileShader);
	
	// Second Player
	if (Game::multiplayer)
	{
		for (Player* player : this->onlinePlayers)
		{
			player->hpBarRenderOptions(true);
			player->render(primitiveShader, skillShader, worldSkillShader, nullptr, projectileShader);
		}
			
	}

	// Monsters
	for (Monster* m : monsters)
	{
		try{
			if (Game::multiplayer)
			{
				if (!localPlayer->isFogOfWar(m->getTransform()->getPosition()) && !m->shouldTranslate())
					m->render(primitiveShader);
			}
			else
				m->render(primitiveShader);
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
	
	// Render GUI (Order is important)
	gui->render();

	cursor->renderCursor(skillShader);
}

void Game::update()
{
	if (Game::multiplayer)
		Chat::updateGameMultiplayer(udpClient, localPlayer, map);
	else
		Chat::updateGameSingleplayer();
	
	// Light update
	light->update(localPlayer->getTransform()->getPosition());
	
	// Player update	
	localPlayer->update(this->map);
	
	// Camera update
	camera->updatePlayer(localPlayer->getTransform()->getPosition());
	frameBuffer->getCamera()->updateLight(light->lightPosition, localPlayer->getTransform()->getPosition());
	
	if (Game::multiplayer)
	{
		for (Player* player : this->onlinePlayers)
			player->update(this->map);
	}
	
	// Monsters update
	for (unsigned int i = 0; i < monsters.size(); i++)
		monsters[i]->update(map, localPlayer);
	
	for (unsigned int i = 0; i < monsters.size(); i++)
		if (monsters[i]->shouldBeDeleted())
		{
			delete monsters[i];
			monsters.erase(monsters.begin() + i);
		}

	for (unsigned int i = 0; i < monsters.size(); i++)
	{
		if (localPlayer->isOutsideExternalRadiusArea(monsters[i]->getTransform()->getPosition()))
			monsters[i]->setShouldTranslate(true);
	}
	
	// GUI update
	gui->update();

	// Cursor update
	cursor->update();
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
}