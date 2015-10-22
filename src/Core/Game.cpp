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
#include "Projectile.h"

#include "GUI.h"
#include "Chat.h"
#include "HPBar.h"
#include "ZurikiRageSkill.h"
#include "HoshoyoExplosionSkill.h"

#include "Packet.h"
#include "PacketController.h"

#include "FrameBuffer.h"

// Standard libs
#include <iostream>
#include <string>

bool Game::multiplayer = true;
int Game::server_port = 9090;
std::string Game::server_ip = "127.0.0.1";
//std::string Game::server_ip = "201.21.40.57";

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

	if (this->localPlayer != nullptr)	delete localPlayer;

	if (Game::multiplayer)
	{
		udpClient->virtualDisconnection();
		if (this->udpClient != nullptr) delete udpClient;
	}
}

void Game::createGraphicElements(int windowWidth, int windowHeight)
{
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
	PacketController::localPlayer = localPlayer;

	this->localPlayer->setSpeedBasis(26);
	this->localPlayer->setMaximumHpBasis(100);
	this->localPlayer->setDefenseBasis(100);
	this->localPlayer->setMagicalPowerBasis(20);
	// TODO delete skill1, 2, 3, 4
	Skill* skill1 = new HoshoyoExplosionSkill(&monsters);
	skill1->setSlot(SLOT_1);
	localPlayer->addNewSkill(skill1);
	
	Skill* skill2 = new HoshoyoExplosionSkill(&monsters);
	skill2->setSlot(SLOT_2);
	localPlayer->addNewSkill(skill2);
	
	Skill* skill3 = new HoshoyoExplosionSkill(&monsters);
	skill3->setSlot(SLOT_3);
	localPlayer->addNewSkill(skill3);
	
	Skill* skill4 = new HoshoyoExplosionSkill(&monsters);
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
	Skill* skill1 = new HoshoyoExplosionSkill(&monsters);
	skill1->setSlot(SLOT_1);
	designedPlayer->addNewSkill(skill1);

	Skill* skill2 = new HoshoyoExplosionSkill(&monsters);
	skill2->setSlot(SLOT_2);
	designedPlayer->addNewSkill(skill2);

	Skill* skill3 = new HoshoyoExplosionSkill(&monsters);
	skill3->setSlot(SLOT_3);
	designedPlayer->addNewSkill(skill3);

	Skill* skill4 = new HoshoyoExplosionSkill(&monsters);
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
	this->gui = new GUI(localPlayer, "./shaders/textshader", "./shaders/fontshader", "./fonts/consola.ttf");
	for (Skill* s : this->localPlayer->getSkills())
		this->gui->addSkillIcon(s->getSkillIcon());
	PacketController::gui = this->gui;
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
				if (m->shouldRender())
				{
					if (!localPlayer->isFogOfWar(m->getTransform()->getPosition()))
						m->render(primitiveShader);
					else
						m->setShouldRender(false);
				}
			}
			else
			{
				m->render(primitiveShader);
			}
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
	localPlayer->render(primitiveShader, gui->getTextRenderer(), projectileShader);

	// Second Player
	if (Game::multiplayer)
	{
		for (Player* player : this->onlinePlayers)
		{
			player->hpBarRenderOptions(false);
			player->render(primitiveShader, gui->getTextRenderer(), projectileShader);
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

	// Monsters
	for (Monster* m : monsters)
	{
		try{
			if (Game::multiplayer)
			{
				if (m->shouldRender())
				{
					if (!localPlayer->isFogOfWar(m->getTransform()->getPosition()))
						m->render(primitiveShader);
					else
						m->setShouldRender(false);
				}
			}
			else
			{
				m->render(primitiveShader);
			}
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
	localPlayer->hpBarRenderOptions(true);
	localPlayer->render(primitiveShader, gui->getTextRenderer(), projectileShader);

	// Second Player
	if (Game::multiplayer)
	{
		for (Player* player : this->onlinePlayers)
		{
			player->hpBarRenderOptions(true);
			player->render(primitiveShader, gui->getTextRenderer(), projectileShader);
		}
			
	}
	// Render GUI (Order is important)
	gui->render();
}

void Game::update()
{
	if (Game::multiplayer)
	{
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
			else if (Chat::msg.compare("/wireframe") == 0)
			{
				Mesh::wireframe = !Mesh::wireframe;
				Chat::msg = "";
			}
			else{

				if (Chat::msg.substr(0, 3).compare("/tp") == 0)
				{
					std::string thisMsg = Chat::msg;
					int firstSpace = thisMsg.find_first_of(" ");
					int secondSpace = thisMsg.find_first_of(" ", firstSpace + 1);
					int endString = thisMsg.length();

					std::string xPos = thisMsg.substr(firstSpace + 1, secondSpace - firstSpace - 1);
					std::string yPos = thisMsg.substr(secondSpace + 1, endString - secondSpace - 1);

					float xPosf = std::stof(xPos);
					float yPosf = std::stof(yPos);

					if (xPosf < MAP_SIZE && xPosf > 0 && yPosf < MAP_SIZE && yPosf > 0)
					{
						if (!map->coordinateHasCollision(glm::vec3(xPosf, yPosf, PLAYER_HEIGHT)))
							localPlayer->getTransform()->translate(xPosf, yPosf, PLAYER_HEIGHT);
						else
							gui->setNextMessage(std::string("Invalid position!"));
					}
					else
						gui->setNextMessage(std::string("Out of the world!"));
				}

				gui->setNextMessage(Chat::appendPlayerName(localPlayer->getName()));
				//gui->setNextMessage(Chat::msg);
				udpClient->sendPackets(Packet(Chat::msg, -1));
				Chat::msg = "";
			}
		}
	}
	else
	{
		if (Chat::msg != "")
		{
			gui->setNextMessage(Chat::msg);
			Chat::msg = "";
		}
	}

	// Light update
	glm::vec3 playerPos = localPlayer->getTransform()->getPosition();
	playerPos.x -= 30.0f;
	playerPos.y -= 30.0f;
	light->update(playerPos);

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
				do {
					randomNumberX = std::rand() % MAP_SIZE;
					randomNumberY = std::rand() % MAP_SIZE;
					newPos = glm::vec3((float)randomNumberX, (float)randomNumberY, 0);
				} while (map->getMapCoordinateForMapCreation(newPos).terrain != NORMAL_FLOOR);

				lastTime = Time::getTime();
				localPlayer->getTransform()->translate(newPos.x, newPos.y, localPlayer->getTransform()->getPosition().z);
			}
		}

		/*if (Input::keyStates['l'])
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
		}*/
#endif
	}
}