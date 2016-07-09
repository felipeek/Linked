// Project Libs
#include <glm\gtc\matrix_transform.hpp>
#include "Game.h"
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
#include "CommonShader.h"
#include "SkillShader.h"
#include "Light.h"

#include "RangeAttack.h"
#include "Projectile.h"

#include "GUI.h"
#include "Chat.h"
#include "Menu.h"
#include "HPBar.h"
#include "HoshoyoExplosionSkill.h"
#include "CureBlessingSkill.h"
#include "SwapSkill.h"
#include "LinkSkill.h"

#include "Packet.h"
#include "PacketController.h"
#include "UDPClient.h"

#include "FrameBuffer.h"
#include "Audio.h"
#include "LinkedDebug.h"

// Standard libs
#include "Logging/Log.h"
#include "Configfiles.h"
#include <string>

bool Game::multiplayer;
int Game::server_port;
std::string Game::server_ip;
Cursor* Game::cursor = nullptr;
Game* Game::current = nullptr;

Game::Game(int windowWidth, int windowHeight)
	: windowWidth(windowWidth), windowHeight(windowHeight)
{
	Config config("config.linked", &multiplayer, &server_port, server_ip, &musicVolume, &effectsVolume);
	PacketController::game = this;
	Game::current = this;

	createGraphicElements(windowWidth, windowHeight);

	createMap();
	initializateAudio();

	if (Game::multiplayer)
	{
		createUDPConnection();
		PacketController::onlinePlayers = &this->onlinePlayers;
		waitForCreationOfOnlinePlayer();
		loadMonstersAndEntities(false, true);
	}
	else
	{
		createOfflinePlayer();
		loadMonstersAndEntities(true, true);
	}

	createGUI();
	Chat::gui = this->gui;
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

	// delete vectors content
	for (Monster* monster : monsters)
		delete monster;
		
	for (Entity* entity : entities)
		delete entity;
	
	for (GameEntity* entity : gameEntities)
		delete entity;

	if (this->localPlayer != nullptr) delete localPlayer;

	if (Game::multiplayer)
	{
		udpClient->virtualDisconnection();
		if (this->udpClient != nullptr) delete udpClient;
	}

	if (this->themeAudio != nullptr) delete this->themeAudio;
	if (this->playerJoinedAudio != nullptr) delete this->playerJoinedAudio;
	if (this->playerDisconnectedAudio != nullptr) delete this->playerDisconnectedAudio;
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
	this->primitiveShader = new PrimitiveShader("./res/shaders/normalshader", camera, light);
	this->commonShader = new CommonShader("./res/shaders/commonshader", camera, light);
	this->projectileShader = new CommonShader("./res/shaders/projectile", camera, light);
	this->mapShader = new MapShader("./res/shaders/mapshader_shadow", camera, light);
	this->skillShader = new SkillShader("./res/shaders/fontshader");
	this->worldSkillShader = new PrimitiveShader("./res/shaders/skillshader", camera, light);

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
#ifdef DEBUG
	localPlayer = new Player(new Transform(glm::vec3(64, 64, PLAYER_HEIGHT), 45, glm::vec3(1, 0, 0), glm::vec3(2, 2, 2)), playerMesh, new Texture("./res/Monsters/Sprites/greenwarrior.png"));
#else
	localPlayer = new Player(new Transform(glm::vec3(189.471f, 104.694f, PLAYER_HEIGHT), 45, glm::vec3(1, 0, 0), glm::vec3(2, 2, 2)), playerMesh, new Texture("./res/Monsters/Sprites/greenwarrior.png"));
#endif
	localPlayer->setHp(100);
	localPlayer->setName("crazyshane75");
	localPlayer->setClientId(0);
	PacketController::localPlayer = localPlayer;

	localPlayer->setAttackBasis(50);
	localPlayer->setSpeedBasis(26);
	localPlayer->setMaximumHpBasis(100);
	localPlayer->setDefenseBasis(100);
	localPlayer->setMagicalPowerBasis(20);

	Skill* skill1 = new LinkSkill(PLAYER);
	skill1->setSlot(SLOT_1);
	localPlayer->addNewSkill(skill1);
	
	Skill* skill2 = new SwapSkill(PLAYER);
	skill2->setSlot(SLOT_2);
	localPlayer->addNewSkill(skill2);
	
	Skill* skill3 = new CureBlessingSkill(PLAYER);
	skill3->setSlot(SLOT_3);
	localPlayer->addNewSkill(skill3);
	
	Skill* skill4 = new HoshoyoExplosionSkill(PLAYER);
	skill4->setSlot(SLOT_4);
	localPlayer->addNewSkill(skill4);

	this->mapShader->setPlayer(localPlayer);
	this->primitiveShader->setPlayer(localPlayer);
	this->worldSkillShader->setPlayer(localPlayer);
}

void Game::waitForCreationOfOnlinePlayer()
{
	double time = LinkedTime::getTime();
	double totalTime = 0;
	do{
		this->udpClient->receivePackets();
		totalTime = LinkedTime::getTime() - time;

		if (totalTime >= 10)
			exit(EXIT_FAILURE);

		Sleep(100);
	}
	while (localPlayer == nullptr);
	//this->localPlayer = PacketController::localPlayer;
}

void Game::createOnlinePlayer(short* data, bool isLocalPlayer)
{
	glm::vec3 localPlayerPosition = glm::vec3(data[8], data[9], PLAYER_HEIGHT);
	// TODO: delete player Mesh
	Mesh* playerMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f, 12, 0));

	// TODO: delete designedPlayer, and his transform/texture

	Player* designedPlayer = new Player(new Transform(localPlayerPosition, 45, glm::vec3(1, 0, 0), glm::vec3(2, 2, 2)), playerMesh, new Texture("./res/Monsters/Sprites/greenwarrior.png"));
	
	designedPlayer->setName("Newbie");
	designedPlayer->setClientId(data[0]);

	designedPlayer->setTotalMaximumHp(data[1]);
	designedPlayer->setHp(data[2]);
	designedPlayer->setTotalAttack(data[3]);
	designedPlayer->setTotalDefense(data[4]);
	designedPlayer->setTotalMagicalPower(data[5]);
	designedPlayer->setTotalSpeed(data[6]);
	designedPlayer->setTotalAttackSpeed(data[7]);

	/* FOR NOW, SKILLS MUST BE THE SAME AS THEY ARE DEFINED SERVER-SIDE (SAME SLOTS, ALSO) */
	Skill* skill1 = new LinkSkill(PLAYER);
	skill1->setSlot(SLOT_1);
	designedPlayer->addNewSkill(skill1);

	Skill* skill2 = new SwapSkill(PLAYER);
	skill2->setSlot(SLOT_2);
	designedPlayer->addNewSkill(skill2);

	Skill* skill3 = new CureBlessingSkill(PLAYER);
	skill3->setSlot(SLOT_3);
	designedPlayer->addNewSkill(skill3);

	Skill* skill4 = new HoshoyoExplosionSkill(PLAYER);
	skill4->setSlot(SLOT_4);
	designedPlayer->addNewSkill(skill4);

	if (isLocalPlayer)
	{
		localPlayer = designedPlayer;
		PacketController::localPlayer = this->localPlayer;
		designedPlayer->setType(LOCAL);
	}
	else
	{
		onlinePlayers.push_back(designedPlayer);
		designedPlayer->setType(NETWORK);
		if (playerJoinedAudio != nullptr) playerJoinedAudio->play();
		if (Chat::gui != nullptr) Chat::gui->setNextMessage(std::string("A new player joined."));
	}
}

void Game::disconnectOnlinePlayer(int* data)
{
	int clientId = data[0];

	for (unsigned int i = 0; i < PacketController::onlinePlayers->size(); i++)
	{
		if ((*PacketController::onlinePlayers)[i]->getClientId() == clientId)
		{
			delete (*PacketController::onlinePlayers)[i];
			(*PacketController::onlinePlayers).erase((*PacketController::onlinePlayers).begin() + i);
			if (playerDisconnectedAudio != nullptr) playerDisconnectedAudio->play();
			if (Chat::gui != nullptr) Chat::gui->setNextMessage(std::string("Player disconnected."));
		}
	}
}

void Game::createGUI()
{
	gui = new GUI(localPlayer);
	for (Skill* s : localPlayer->getSkills())
		gui->addSkillIcon(s->getSkillIcon());
	PacketController::gui = gui;
	Chat::gui = gui;	
}

void Game::initializateAudio()
{
	Audio::setMusicVolume(musicVolume);
	Audio::setSoundVolume(effectsVolume);
	themeAudio = new Audio(THEME_AUDIO_PATH, AudioType::MUSIC);
	themeAudio->setLoop(true);
	themeAudio->play();
	playerJoinedAudio = new Audio(PLAYER_JOINED_AUDIO_PATH, AudioType::SOUND);
	playerDisconnectedAudio = new Audio(PLAYER_DISCONNECTED_AUDIO_PATH, AudioType::SOUND);
}

void Game::loadMonstersAndEntities(bool loadMonsters, bool loadEntities)
{
	// Note: 11 seconds to load map
	Mesh* mapMesh = new Mesh(new Grid(MAP_SIZE, map));

	entityMap = new EntityMap(new Transform(), mapMesh,
		new Texture("./res/Maps/snow.jpg"),
		new Texture("./res/Maps/ice_mountain.jpg"),
		new Texture("./res/Maps/water.jpg"),
		new Texture("./res/Maps/mudpath.jpg"),
		new Texture(TERRAIN_MAP_PATH));
	entityMap->setShadowTexture(frameBuffer->getTexture());


	Mesh* waterMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), (float)MAP_SIZE, (float)MAP_SIZE));
	Texture* waterTexture = new Texture("./res/Maps/water.bmp");
	waterTexture->setTileAmount(100);
	water = new Entity(new Transform(glm::vec3(0, 0, -1.0f)), waterMesh, waterTexture);

	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			MapCoordinate coordinate = map->getMapCoordinateForMapCreation(glm::vec3(i, j, 0));
			Monster *monster = coordinate.mapMonster.monster;
			GameEntity *gameEntity = coordinate.mapGameEntity.gameEntity;
			
			if (coordinate.mapMonster.monsterExists)
			{
				if (!map->coordinateHasCollision(glm::vec3(i, j, 0)) && loadMonsters)
				{
					monster->getTransform()->translate((float)i, (float)j, 1.3f);
					monsters.push_back(monster);
				}
				else
					delete monster;
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
	// do not tests collision intentionally
	int monsterId = data[0];
	int monsterHp = data[1];
	glm::vec3 monsterRgb = glm::vec3(data[2], data[3], data[4]);
	glm::vec3 monsterPosition = glm::vec3(data[5], data[6], data[7]);

	Monster* newMonster = monsterFactory->getMonsterOfMapColor(monsterRgb);
	newMonster->getTransform()->translate(monsterPosition.x, monsterPosition.y, monsterPosition.z);
	newMonster->setId(monsterId);
	newMonster->setHp(monsterHp);
	monsters.push_back(newMonster);
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
	
	// Render GUI and Cursor (Order is important)
	bool wire = Mesh::wireframe;
	Mesh::wireframe = false;
	gui->render();
	cursor->renderCursor(skillShader);
	Mesh::wireframe = wire;
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
		if (Game::multiplayer)
		{
			if (!localPlayer->isFogOfWar(m->getTransform()->getPosition()) && !m->shouldTranslate())
				m->render(primitiveShader, skillShader, nullptr);
		}
		else
			m->render(primitiveShader, skillShader, nullptr);
	}

	// Common static entities
	for (Entity* e : gameEntities)
			e->render(commonShader);

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
		if (Game::multiplayer)
		{
			if (!localPlayer->isFogOfWar(m->getTransform()->getPosition()) && !m->shouldTranslate())
				m->render(primitiveShader, skillShader, nullptr);
		}
		else
			m->render(primitiveShader, skillShader, nullptr);
	}

	// Common static entities
	for (Entity* e : gameEntities)
			e->render(commonShader);
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
	localPlayer->update(this->map, this->localPlayer, &this->onlinePlayers, &this->monsters);

	// Camera update
	camera->updatePlayer(localPlayer->getTransform()->getPosition());
	frameBuffer->getCamera()->updateLight(light->lightPosition, localPlayer->getTransform()->getPosition());

	if (Game::multiplayer)
	{
		for (Player* player : this->onlinePlayers)
			player->update(this->map, this->localPlayer, &this->onlinePlayers, &this->monsters);
	}

	// Monsters update
	for (unsigned int i = 0; i < monsters.size(); i++)
		monsters[i]->update(map, localPlayer, &this->monsters);

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