#include "Chat.h"
#include "UDPClient.h"
#include "Player.h"
#include "Map.h"
#include "Game.h"
#include "Packet.h"
#include "Input.h"
#include "GUI.h"
#include <Windows.h>
#include <iostream>

int Chat::stateChat[NUM_KEYS] = { 0 };
short Chat::capsLockState = 0;
short Chat::numLockState = 0;
std::stringstream Chat::ss;
std::string Chat::msg = "";
bool Chat::chatActive = false;
bool Chat::m_enabled = true;
GUI* Chat::gui = nullptr;
int Chat::altGRstate = 0;
int Chat::m_restriction = CHAT_ALL;
int Chat::m_maxLength = 255;

void Chat::nextState()
{
	if (stateChat[RETURN_KEY] == 3)
		stateChat[RETURN_KEY] = 0;
	else
		stateChat[RETURN_KEY]++;
}

void Chat::nextState(int key)
{

}

void Chat::update(int key, int scancode, int action, int mods)
{
	if (m_enabled)
	{
		// Chat system
		if ((key == RETURN_KEY && action == 0) || (key == RETURN_KEY && action == 1) ||
			(key == RETURN_KEY_NUM && action == 0) || (key == RETURN_KEY_NUM && action == 1))
			Chat::nextState();
		else if (action == 0 || action == 1)
			Chat::nextState(key);
	}
	if (stateChat[RETURN_KEY] == CHAT_ACTIVE)
	{
		chatActive = true;
		if (action == 1 || action == 2)
		{
			// Fill string stream with character typed if shorter than length
			if (Chat::ss.str().size() < m_maxLength || key == BACKSPACE_KEY)
				parseCharTyped(key, mods, scancode);
		}
	}
	if (stateChat[RETURN_KEY] == NORMAL_INACTIVE)
	{
		//std::cout << "Chat desativado" << std::endl;
		chatActive = false;
		if (ss.rdbuf()->in_avail() != 0)
		{
			msg = ss.str();
			ss.clear();
			ss.str(std::string());
		}
	}
}

void Chat::disable()
{
	m_enabled = false;
	chatActive = true;
	stateChat[RETURN_KEY] = CHAT_ACTIVE;
	if (ss.rdbuf()->in_avail() != 0)
	{
		ss.clear();
		ss.str(std::string());
	}
}

void Chat::enable()
{
	m_enabled = true;
	chatActive = false;
	stateChat[RETURN_KEY] = NORMAL_INACTIVE;
	if (ss.rdbuf()->in_avail() != 0)
	{
		ss.clear();
		ss.str(std::string());
	}
	Chat::setRestriction(CHAT_ALL);
	Chat::setMaxMsgLength(MAX_CHAT_LENGTH);
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
		if (Chat::msg.compare("/wireframe") == 0)
		{
			Mesh::wireframe = !Mesh::wireframe;
			Chat::msg = "";
		} else if (gui != nullptr)
			gui->setNextMessage(Chat::msg);
		Chat::msg = "";
	}
}

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

void Chat::setRestriction(ChatRestriction r)
{
	m_restriction = (int)r;
}

void Chat::setMaxMsgLength(int maxLength)
{
	m_maxLength = maxLength;
}

void Chat::parseCharTyped(int key, int mods, int scancode)
{
	if (key == -1 && m_restriction == CHAT_ALL)
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
		if (isLetter(key) && m_restriction != CHAT_NUMBER_ONLY)
		{
			char c = getLetter(key, mods);
			ss << c;
		}
		// Spacebar
		else if (key == SPACE_KEY && m_restriction != CHAT_NUMBER_ONLY)
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
		if (m_restriction == CHAT_ALL)
		{
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
			case KEY_CCEDILHA: ss << "ç"; break;
			case KEY_TILDA: ss << "~"; break;
			case KEY_ACUTE_ACCENT: ss << "´"; break;
			//Numpad
			case KEY_STAR_NUM: ss << "*"; break;
			case KEY_MINUS_NUM: ss << "-"; break;
			case KEY_PLUS_NUM: ss << "+"; break;
			case KEY_COMMA_NUM: ss << ","; break;
			}
		}
	}
	else if (mods == KEY_MOD_SHIFT && m_restriction == CHAT_ALL)
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
		case KEY_6: ss << "¨"; break;
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
		case KEY_CCEDILHA: ss << "Ç"; break;
		}
	}
	else if (mods == KEY_MOD_CONTROL && m_restriction == CHAT_ALL)
	{

	}
	else if (mods == KEY_MOD_ALT && m_restriction == CHAT_ALL)
	{

	}
	else if (mods == KEY_MOD_ALTGR && m_restriction == CHAT_ALL)
	{
		switch (key)
		{
		case 'W': ss << "?"; break;
		}
	}
	// Check keys

}