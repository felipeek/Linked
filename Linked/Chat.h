#pragma once

#include <string>
#include <sstream>

#define NUM_KEYS 512

#define RETURN_KEY 257
#define SPACE_KEY 32
#define BACKSPACE_KEY 259

class Chat
{
public:
	Chat();
	~Chat();
	static std::string msg;
	static void update(int key, int scancode, int action, int mods);
private:
	static int stateChat[NUM_KEYS];	// 0 to 3
	// Normal 0, Normal 1, Chat 0, Chat 1
	static void nextState(int key);
	static void nextState();
	static void printState(int key);

	static bool isLetter(int key);
	static bool isNumber(int key, int mods);

	static char getLetter(int key, int mods);
	static char getNumber(int key);
	static short capsLockState;		// 0 deactive, 1 active
	static short numLockState;

	static std::stringstream ss;
};

