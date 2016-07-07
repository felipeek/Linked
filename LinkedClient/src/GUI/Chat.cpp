#include "Chat.h"
#include "UDPClient.h"
#include "Packet.h"
#include "Input.h"
#include "Player.h"
#include "Map.h"
#include "GUI.h"
#include "Game.h"
#include "LinkedDebug.h"
#include <iostream>

std::string Chat::msg = "";
std::stringstream Chat::ss;
bool Chat::m_Enabled = false;
GUI* Chat::gui = nullptr;

LinkedDebug d;

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
				gui->setNextMessage(Chat::AppendPlayerName(localPlayer->getName()));
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

		if (Chat::msg[0] == '/')
		{
			switch (Chat::msg[1])
			{
				case 'd': {
					char varbuffer[256] = {};
					char argbuffer[256] = {};
					int i = 3;
					for (; Chat::msg[i] != 0 && Chat::msg[i] != ' '; ++i)
						varbuffer[i - 3] = Chat::msg[i];
					if (Chat::msg[i] != 0)
					{
						i++;
						for (int j = 0; Chat::msg[i] != 0 && Chat::msg[i] != ' '; ++i, ++j)
							argbuffer[j] = Chat::msg[i];
					}
					int address = strtol(varbuffer, NULL, 16);
					int offset = atoi(argbuffer);
					gui->setNextMessage(d.PrintAtAddress((void*)address, offset));
				}break;
			}
		}

		if (Chat::msg[0] == '/')
		{
			switch (Chat::msg[1])
			{
			case 's': {
				char varbuffer[256] = {};
				char argbuffer[256] = {};
				char valbuffer[256] = {};
				int i = 3;
				for (; Chat::msg[i] != 0 && Chat::msg[i] != ' '; ++i)
					varbuffer[i - 3] = Chat::msg[i];
				if (Chat::msg[i] != 0)
				{
					i++;
					for (int j = 0; Chat::msg[i] != 0 && Chat::msg[i] != ' '; ++i, ++j)
						argbuffer[j] = Chat::msg[i];
				}
				if (Chat::msg[i] != 0)
				{
					i++;
					for (int j = 0; Chat::msg[i] != 0 && Chat::msg[i] != ' '; ++i, ++j)
						valbuffer[j] = Chat::msg[i];
				}

				int address = strtol(varbuffer, NULL, 16);
				int offset = atoi(argbuffer);
				int value = atoi(valbuffer);
				d.SetIntegerValueAtAddress((void*)address, offset, value);
			}break;
			}
		}

		Chat::msg = "";
	}
}

void Chat::ProcessKeyStroke(int key)
{
	if (key == 13)	// Enter
	{
		if (m_Enabled)
		{
			Chat::msg = ss.str();
			ss.str("");
		}
		m_Enabled = !m_Enabled;
	}
	else 
	{
		if (key == 8)
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
		if (m_Enabled && key >= 10)
		{
			ss << (char)key;
		}
	}
}

std::string Chat::AppendPlayerName(std::string& playerName)
{
	std::string s = playerName +": ";
	s.append(Chat::msg);
	return s;
}