#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>

#include "Packet.h"
#include "ClientPacket.h"
#include "ClientInfo.h"

#pragma comment (lib, "Ws2_32.lib")

#define RECVBUFFERSIZE 512

class UDPServer
{
public:
	UDPServer(int port);
	~UDPServer();
	void receivePackets();
	void sendPackets(Packet& packet, struct sockaddr_in& client);
	std::vector <ClientInfo*>* getClients();
	int clientExists(int id);
	int verifyClient(struct sockaddr_in* client);
	int addToClients(struct sockaddr_in* client);

	double getPing();
private:
	int port;
	WSADATA wsaData;

	struct sockaddr_in serverInfo, clientInfo;
	int sizeServerInfo, sizeClientInfo;

	std::vector <ClientInfo*> clientsInfo;

	SOCKET connectionSocket;

	bool asyncSocket;

	// Methods
	void startWinsock();
	void createSocket();
	void initAndBindSocket();
	friend bool operator==(sockaddr_in l, sockaddr_in r);
	friend bool operator!=(sockaddr_in l, sockaddr_in r);

	double last;

	double ping;
};

