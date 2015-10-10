#pragma once
#include "Configuration.h"
#include <vector>
#include "network\UDPClient.h"
#include <glm\glm.hpp>

#define MAP_SIZE 1024

#define TERRAIN_MAP_PATH "./res/Maps/teste.png"
#define ENTITIES_MAP_PATH "./res/Maps/entities.png"
#define MONSTER_MAP_PATH "./res/Maps/monsters.png"

class EntityMap;
class Entity;
class Camera;
class PrimitiveShader;
class CommonShader;
class GUIShader;
class MapShader;
class TextShader;
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

class Game
{
public:
	Game();
	Game(int windowsWidth, int windowsHeight);
	~Game();

	/* INITIALIZATION METHODS */
	void createOnlinePlayer(short* data, bool isLocalPlayer);

	/* RENDER & UPDATE*/
	void render();
	void update();

private:

	/* INITIALIZATION METHODS */
	void createGraphicElements(int windowsWidth, int windowsHeight);
	void createMap();
	void createOfflinePlayer();
	void waitForCreationOfOnlinePlayer();
	void createGUI();
	void createUDPConnection();
	void loadMonstersAndEntities(bool loadMonsters, bool loadEntities);

	/* GAME GRAPHIC ELEMENTS */
	Camera* camera;
	Light* light;
	CommonShader* commonShader;
	PrimitiveShader* primitiveShader;
	MapShader* mapShader;
	CommonShader* projectileShader;
	GUI* gui;
	TextRenderer* textRenderer;

	/* GAME MAP & RELATED */
	EntityMap* entityMap;
	Map* map;
	MonsterFactory* monsterFactory;
	GameEntityFactory* gameEntityFactory;

	/* GAME PLAYERS AND ATTACK */
	Player* localPlayer = NULL;
	RangeAttack* localPlayerRangeAttack = NULL;
#ifdef MULTIPLAYER
	Player* secondPlayer = NULL;
	RangeAttack* secondPlayerRangeAttack = NULL;
#endif

	/* GAME ENTITIES VECTORS */
	std::vector<Entity*> entities;
	std::vector<Projectile*> localPlayerAttacks;
	std::vector<GameEntity*> gameEntities;
	std::vector<Monster*> monsters;
#ifdef MULTIPLAYER
	std::vector<Projectile*> secondPlayerAttacks;
#endif

	/* GAME NETWORK */
	UDPClient* udpClient;

	/* GAME INPUT */
	void input();
	double lastTime;
};

