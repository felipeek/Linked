#pragma once

#include <string>
#include <sstream>

#define NUM_KEYS 512

#define RETURN_KEY 257
#define ESCAPE_KEY 256
#define RETURN_KEY_NUM 335
#define SPACE_KEY 32
#define BACKSPACE_KEY 259

/* CHAT STATES */
#define NORMAL_INACTIVE 0
#define NORMAL_ACTIVE 1
#define CHAT_ACTIVE 2
#define CHAT_INACTIVE 3

/* KEY MODS */
#define NO_MOD 0
#define KEY_MOD_SHIFT 1
#define KEY_MOD_CONTROL 2
#define KEY_MOD_ALT 4
#define KEY_MOD_ALTGR 6
#define KEY_MOD_SUPER 8

class UDPClient;
class Player;
class Map;
class GUI;

class Chat
{
public:
	static bool isChatActive();
	static std::stringstream& getStream();
	static std::string appendPlayerName(std::string& name);
	static std::string msg;
	static void updateGameMultiplayer(UDPClient* udpClient, Player* localPlayer, Map* map);
	static void updateGameSingleplayer();
	static void update(int key, int scancode, int action, int mods);

	//GUI
	static GUI* gui;
private:
	static bool chatActive;
	static int stateChat[NUM_KEYS];	// 0 to 3
	// Normal 0, Normal 1, Chat 0, Chat 1
	static void nextState(int key);
	static void nextState();
	static void printState(int key);

	static bool isLetter(int key);
	static bool isNumber(int key, int mods);

	static void parseCharTyped(int key, int mods, int scancode);

	static char getLetter(int key, int mods);
	static char getNumber(int key);
	static short capsLockState;		// 0 deactive, 1 active
	static short numLockState;

	static std::stringstream ss;

	static int altGRstate;
};

