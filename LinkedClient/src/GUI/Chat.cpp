#include "Chat.h"
#include "UDPClient.h"
#include "Packet.h"
#include "Input.h"
#include "Player.h"
#include "Map.h"
#include "GUI.h"
#include "Game.h"
#include <iostream>

std::string Chat::msg = "";
std::stringstream Chat::ss;
bool Chat::m_Enabled = false;
GUI* Chat::gui = nullptr;


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