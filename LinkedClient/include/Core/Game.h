#pragma once

#include "UDPClient.h"

#include <glm\glm.hpp>

#include <vector>

#define MAP_SIZE 1024

#define TERRAIN_MAP_PATH "./res/Maps/map_64.png"
#define ENTITIES_MAP_PATH "./res/Maps/entities2.png"
#define MONSTER_MAP_PATH "./res/Maps/monsters.png"

#define SHADOW_BUFFER_SIZE 2048

#define PLAYER_HEIGHT 1.39f

class EntityMap;
class Entity;
class Camera;
class PrimitiveShader;
class CommonShader;
class GUIShader;
class MapShader;
class TextShader;
class SkillShader;
class Map;
class Light;
class MonsterFactory;
class Monster;
class RangeAttack;
class Projectile;
class GameEntityFactory;
class GameEntity;
class Player;
class GUI;
class TextRenderer;
class FrameBuffer;
class Shader;
class Cursor;

class Game
{
public:
	Game();
	Game(int windowWidth, int windowHeight);
	~Game();
	static bool multiplayer;
	static int server_port;
	static std::string server_ip;

	/* INITIALIZATION METHODS */
	void createOnlinePlayer(short* data, bool isLocalPlayer);
	void createMonster(short* data);

	/* AUXILIAR METHODS */
	Monster* getMonsterOfId(int id);
	void destroyProjectileOfId(int id);
	static void showCursor(bool show);

	/* RENDER & UPDATE*/
	void render();
	void update();

	/* GAME INPUT */
	void input();
private:
	int windowWidth;
	int windowHeight;
	/* INITIALIZATION METHODS */
	void createGraphicElements(int windowsWidth, int windowsHeight);
	void createMap();
	void createOfflinePlayer();
	void waitForCreationOfOnlinePlayer();
	void createGUI();
	void createUDPConnection();
	void loadMonstersAndEntities(bool loadMonsters, bool loadEntities);

	/* GAME GRAPHIC ELEMENTS */
	Cursor* cursor;
	Camera* camera;
	Light* light;
	CommonShader* commonShader;
	PrimitiveShader* primitiveShader;
	MapShader* mapShader;
	CommonShader* projectileShader;
	SkillShader* skillShader;
	GUI* gui;
	TextRenderer* textRenderer;
	FrameBuffer* frameBuffer;
	Entity* water;

	/* GAME MAP & RELATED */
	EntityMap* entityMap;
	Map* map;
	MonsterFactory* monsterFactory;
	GameEntityFactory* gameEntityFactory;

	/* GAME PLAYERS AND ATTACK */
	Player* localPlayer = NULL;
	std::vector<Player*> onlinePlayers;

	/* GAME ENTITIES VECTORS */
	std::vector<Entity*> entities;
	std::vector<GameEntity*> gameEntities;
	std::vector<Monster*> monsters;

	/* GAME NETWORK */
	UDPClient* udpClient;

	/* GAME RENDER PASSES */
	void renderFirstPass();
	void renderSecondsPass();

	static bool mShowCursor;
	double cursorRot = 0;
	double lastTime;
};

