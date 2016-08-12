#pragma once

#include <glm\glm.hpp>
#include <vector>

#define DEBUG

#ifdef DEBUG
#define MAP_SIZE 128
#else
#define MAP_SIZE 1024
#endif

#ifdef DEBUG
#define TERRAIN_MAP_PATH "./res/Maps/debug_terrain_map.png"
#define ENTITIES_MAP_PATH "./res/Maps/debug_entities_map.png"
#define MONSTER_MAP_PATH "./res/Maps/debug_monsters_map.png"
#else
#define TERRAIN_MAP_PATH "./res/Maps/terrain_map.png"
#define ENTITIES_MAP_PATH "./res/Maps/entities_map.png"
#define MONSTER_MAP_PATH "./res/Maps/monsters_map.png"
#endif

#define SHADOW_BUFFER_SIZE 4096

#define PLAYER_HEIGHT 1.39f

class EntityMap;
class Entity;
class Camera;
class PrimitiveShader;
class CommonShader;
class GUIShader;
class MapShader;
class SkillShader;
class Map;
class Light;
class MonsterFactory;
class Monster;
class GameEntityFactory;
class GameEntity;
class Player;
class GUI;
class FrameBuffer;
class Cursor;
class UDPClient;

class Game
{
public:
	bool running;
	Game();
	Game(int windowWidth, int windowHeight);
	~Game();
	static bool multiplayer;
	static int server_port;
	static std::string server_ip;

	static Cursor* cursor;

	/* INITIALIZATION METHODS */
	void createOnlinePlayer(short* data, bool isLocalPlayer);
	void createMonster(short* data);

	/* AUXILIAR METHODS */
	Monster* getMonsterOfId(int id);
	void destroyProjectileOfId(int id);
	void disconnectOnlinePlayer(int* data);

	/* RENDER & UPDATE*/
	void render();
	void renderGUI();
	void update();

	/* GAME INPUT */
	void input();

private:
	int musicVolume;
	int effectsVolume;

	static Game* current;
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
	void initializateAudio();

	/* GAME GRAPHIC ELEMENTS */
	Camera* camera;
	Light* light;
	CommonShader* commonShader;
	PrimitiveShader* primitiveShader;
	PrimitiveShader* worldSkillShader;
	MapShader* mapShader;
	CommonShader* projectileShader;
	SkillShader* skillShader;
	GUI* gui;
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

	double cursorRot = 0;
	double lastTime;
public:
	static inline Game* getCurrent() { return current; }
	inline int getWidth()const { return windowWidth; }
	inline int getHeight()const { return windowHeight; }
	inline void setWindowWidth(int width) { windowWidth = width; }
	inline void setWindowHeight(int height) { windowHeight = height; }
	inline GUI* getGui(){ return this->gui; }
};

