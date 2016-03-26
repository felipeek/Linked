#pragma once
#include <string>
#include <sstream>

class UDPClient;
class Player;
class Map;
class GUI;

class Chat
{
public:
	static void updateGameMultiplayer(UDPClient* udpClient, Player* localPlayer, Map* map);
	static void updateGameSingleplayer();
	
	static void ProcessKeyStroke(int key);

	static GUI* gui;
private:
	static std::string AppendPlayerName(std::string& playerName);
	static bool m_Enabled;
	static std::stringstream ss;
	static std::string msg;

public:
	inline static bool isChatActive() { return m_Enabled; }
	inline static std::stringstream& getStream() { return ss; }
};

