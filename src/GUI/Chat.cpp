#include "Chat.h"
#include "UDPClient.h"
#include "Player.h"
#include "Game.h"
#include "Map.h"
#include "Packet.h"
#include <iostream>
#include <Windows.h>

int Chat::stateChat[NUM_KEYS] = { 0 };
short Chat::capsLockState = 0;
short Chat::numLockState = 0;
std::stringstream Chat::ss;
std::string Chat::msg = "";
bool Chat::chatActive = false;
GUI* Chat::gui = nullptr;

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
	std::cout <<"Scancode: "<< scancode <<", key: "<< key << std::endl;
	// Chat system
	if ((key == RETURN_KEY && action == 0) || (key == RETURN_KEY && action == 1) ||
		(key == RETURN_KEY_NUM && action==0) || (key == RETURN_KEY_NUM && action == 1))
		Chat::nextState();
	else if (action == 0 || action == 1)
		Chat::nextState(key);

	if (stateChat[RETURN_KEY] == CHAT_ACTIVE)
	{
		chatActive = true;
		if (action == 1 || action == 2)
		{
			// Fill string stream with character typed
			parseCharTyped(key, mods, scancode);
		}
	}
	if (stateChat[RETURN_KEY] == NORMAL_INACTIVE)
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

void Chat::updateGameMultiplayer(UDPClient* udpClient, Player* localPlayer, Map* map)
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

				try{
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
				catch (...){
					gui->setNextMessage(std::string("Invalid input!"));
				}
				Chat::msg = "";
			}
			else
			{
				gui->setNextMessage(Chat::appendPlayerName(localPlayer->getName()));
				//gui->setNextMessage(Chat::msg);
				udpClient->sendPackets(Packet(Chat::msg, -1));
				Chat::msg = "";
			}
		}
	}
	
}

void Chat::updateGameSingleplayer()
{
	if (Chat::msg != "")
	{
		if (gui != nullptr)
			gui->setNextMessage(Chat::msg);
		Chat::msg = "";
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

void Chat::parseCharTyped(int key, int mods, int scancode)
{
	if (key == -1)
	{
		// Keys that are not recognized
		if (scancode == FSLASH_SCANCODE && mods == NO_MOD)
			ss << "/";
		else if (scancode == FSLASH_SCANCODE && mods == KEY_MOD_SHIFT)
			ss << "?";

	}

	if (mods == KEY_MOD_SHIFT || mods == NO_MOD)
	{
		// Equal behavior when shifted
		// Letters
		if (isLetter(key))
		{
			char c = getLetter(key, mods);
			ss << c;
		}
		// Spacebar
		else if (key == SPACE_KEY)
		{
			ss << " ";
		}
		// Backspace
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
	}
	if (mods == NO_MOD)
	{
		// Numbers
		if (isNumber(key, mods))
		{
			char c = getNumber(key);
			ss << c;
		}
		switch (key)
		{
		case KEY_FSLASH: ss << "/"; break;
		case KEY_APOSTROPHE: ss << "'"; break;
		case KEY_COMMA: ss << ","; break;
		case KEY_MINUS: ss << "-"; break;
		case KEY_PERIOD: ss << "."; break;
		case KEY_SEMICOLON: ss << ";"; break;
		case KEY_EQUAL: ss << "="; break;
		case KEY_LEFTBRACKET: ss << "["; break;
		case KEY_RIGHTBRACKET: ss << "]"; break;
		case KEY_BACKSLASH: ss << "\\"; break;
		//case KEY_CCEDILHA: ss << "ç"; break;		// Breaks the textRenderer
		//Numpad
		case KEY_STAR_NUM: ss << "*"; break;
		case KEY_MINUS_NUM: ss << "-"; break;
		case KEY_PLUS_NUM: ss << "+"; break;
		case KEY_COMMA_NUM: ss << ","; break;
		}
		std::string ccc = ss.str();
	}
	else if (mods == KEY_MOD_SHIFT)
	{
		// Forward slash (Left keyboard)

		switch (key)
		{
		// Numeric keys
		case KEY_0: ss << ")"; break;
		case KEY_1: ss << "!"; break;
		case KEY_2: ss << "@"; break;
		case KEY_3: ss << "#"; break;
		case KEY_4: ss << "$"; break;
		case KEY_5: ss << "%"; break;
		//case KEY_6: ss << "¨"; break;		// Breaks the chat
		case KEY_7: ss << "&"; break;
		case KEY_8: ss << "*"; break;
		case KEY_9: ss << "("; break;

		// Special keys
		case KEY_APOSTROPHE: ss << "\""; break;
		case KEY_COMMA: ss << "<"; break;
		case KEY_MINUS: ss << "_"; break;
		case KEY_PERIOD: ss << ">"; break;
		case KEY_SEMICOLON: ss << ":"; break;
		case KEY_EQUAL: ss << "+"; break;
		case KEY_LEFTBRACKET: ss << "{"; break;
		case KEY_RIGHTBRACKET: ss << "}"; break;
		case KEY_BACKSLASH: ss << "|"; break;
		//case KEY_CCEDILHA: ss << "Ç"; break;		// Breaks the textRenderer
		}
	}
	else if (mods == KEY_MOD_CONTROL)
	{

	}
	else if (mods == KEY_MOD_ALT)
	{

	}
	// Check keys

}