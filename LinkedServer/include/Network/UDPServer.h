#pragma once

#define WIN32_LEAN_AND_MEAN

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
    int sockfd; 
	std::vector <ClientInfo*> clientsInfo;
	friend bool operator==(struct sockaddr_in l, struct sockaddr_in r);
	friend bool operator!=(struct sockaddr_in l, struct sockaddr_in r);
};

