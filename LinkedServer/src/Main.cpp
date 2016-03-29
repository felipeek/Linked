#include <iostream>
#include "glm\glm.hpp"
#include "network\UDPServer.h"
#include "LinkedTime.h"
#include "PacketController.h"
#include "Game.h"
#include <thread>

#define PORT 9090

#define MAX_DC_TIME 5

#define CONNECTION_CHECK_TIME 5

using namespace std;

int main(int argc, char** argv)
{
	UDPServer* server = new UDPServer(PORT);
	PacketController::udpServer = server;
	Game* game = new Game();

	srand((unsigned int)time(NULL));

	double totalTime = 0;
	double lastTime = 0;
	double timePassed = 0;
	double cumulativeTime = 0;
	double pingTime = 0;
	double gameTime = 0;
	double update10Time = 0;

	do{
		server->receivePackets();
		totalTime = LinkedTime::getTime();

		if (lastTime == 0)
			lastTime = totalTime;

		timePassed = totalTime - lastTime;
		lastTime = totalTime;

		cumulativeTime += timePassed;
		pingTime += timePassed;
		gameTime += timePassed;
		update10Time += timePassed;

		// Ping every two seconds
		if (pingTime >= 5.0)
		{
			for (unsigned int i = 0; i < server->getClients()->size(); i++)
			{

				double pingToLastPong = (*server->getClients())[i]->lastPing - (*server->getClients())[i]->lastPong;
			//	cout << "pingToLastPong: do cliente " << (*server->getClients())[i]->id << ": " << pingToLastPong << "." << endl;
			//	cout << "Client (" << inet_ntoa((*server->getClients())[i]->netInfo.sin_addr) <<
			//		":" << (*server->getClients())[i]->netInfo.sin_port <<
			//		") ping: " << (*server->getClients())[i]->getPing() << " ms " << endl; //<< pingToLastPong << endl;
				if (pingToLastPong >= MAX_DC_TIME && pingToLastPong <= MAX_DC_TIME * 2)
				{
					// end of connection for time out
					PacketController::disconnectClient((*server->getClients())[i]->id, true);
				}
			}
			pingTime = 0;
		}

		// Connection check
		if (cumulativeTime >= CONNECTION_CHECK_TIME)
		{
			for (unsigned int i = 0; i < server->getClients()->size(); i++)
			{
				server->sendPackets(Packet(ID_PING, 0), (*server->getClients())[i]->netInfo);
				(*server->getClients())[i]->lastPing = LinkedTime::getTime();
			}
			cumulativeTime = 0;
		}

		// Game Update
		if (gameTime >= 1.0 / GAMESPEED)			// Updates GAMESPEED times per second
		{
			game->update();
			gameTime = gameTime - (1.0 / GAMESPEED);
		}

		// Update 10 seconds
		if (update10Time >= 1.0 / 5)	// Updates 10 times per second
		{
			PacketController::update10();
			update10Time = 0;
		}

		//Sleep(100);
	} while (true);

	return 0;
}