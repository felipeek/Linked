#include "Chat.h"
#include <iostream>
#include <Windows.h>

int Chat::stateChat[NUM_KEYS] = { 0 };
short Chat::capsLockState = 0;
short Chat::numLockState = 0;
std::stringstream Chat::ss;
std::string Chat::msg = "";
bool Chat::chatActive = false;

void Chat::nextState()
{
	if (stateChat[RETURN_KEY] == 3)
		stateChat[RETURN_KEY] = 0;
	else
		stateChat[RETURN_KEY]++;

	//printState(key);
}

void Chat::nextState(int key)
{

}

void Chat::update(int key, int scancode, int action, int mods)
{
	// Chat system
	if (key == RETURN_KEY && action == 0 || key == RETURN_KEY && action == 1)
		Chat::nextState();
	else if (action == 0 || action == 1)
		Chat::nextState(key);

	if (stateChat[RETURN_KEY] == 2)
	{
		chatActive = true;
		if (action == 1 || action == 2)
		{
			// Check keys
			if (isLetter(key))
			{
				char c = getLetter(key, mods);
				std::cout << c;
				ss << c;
			}
			else if (isNumber(key, mods))
			{
				char c = getNumber(key);
				std::cout << c;
				ss << c;
			}
			else if (key == SPACE_KEY)
			{
				std::cout << " ";
				ss << " ";
			}
			else if (key == BACKSPACE_KEY)
			{
				std::string s = ss.str();
				if (s.size() > 0)
				{
					ss.clear();
					s.erase(s.end() - 1, s.end());
					ss.str(s);
					ss.seekp(0, std::ios_base::end);
				}
			}
			else if (key == '/')
			{
				std::cout << '/';
				ss << '/';
			}
		}
	}
	if (stateChat[RETURN_KEY] == 0)
	{
		chatActive = false;
		//std::cout << "Chat desativado" << std::endl;
		if (ss.rdbuf()->in_avail() != 0)
		{
			//std::cout << std::endl << ss.str() << std::endl;
			msg = ss.str();
			ss.clear();
			ss.str(std::string());
		}
	}
}
#ifdef DEBUG
void Chat::printState(int key)
{
	switch (stateChat[key])
	{
	case 0:	std::cout << "Normal 0" << std::endl; break;
	case 1:	std::cout << "Normal 1" << std::endl; break;
	case 2:	std::cout << "Chat 0" << std::endl; break;
	case 3:	std::cout << "Chat 1" << std::endl; break;
	}
}
#endif

bool Chat::isLetter(int key)
{
	if (key >= 65 && key <= 90)
		return true;
	else
		return false;
}

bool Chat::isNumber(int key, int mods)
{
	numLockState = GetKeyState(VK_NUMLOCK);

	if (key >= 48 && key <= 57 && mods != 1)
		return true;
	else if(key >= 320 && key <= 329 && mods != 1 && numLockState == 1)
		return true;
	else
		return false;
}

char Chat::getLetter(int key, int mods)
{
	// key is <= 90 and >=65 ( A ~ Z )
	capsLockState = GetKeyState(VK_CAPITAL);

	// if caps lock is inactive and shift is up
	// if caps lock is active and shift is down
	if (capsLockState == 0 && mods != 1 || capsLockState == 1 && mods == 1)
	{
		// lower case
		return (char)(key + 32);
	}
	else{
		return (char)key;
	}
}

char Chat::getNumber(int key)
{
	// key is >= 48 and <= 57 or >= 320 and <= 329 if numeric pad
	if (key >= 48 && key <= 57)
		return (char)key;
	else
		return (char)(key - 272);
}

bool Chat::isChatActive()
{
	return chatActive;
}

std::stringstream& Chat::getStream()
{
	return ss;
}

std::string Chat::appendPlayerName(std::string& name)
{
	std::stringstream stream;
	stream << name << ": "<< msg;
	return stream.str();
}