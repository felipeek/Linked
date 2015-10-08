#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#pragma comment (lib, "Ws2_32.lib")

#define RECVBUFFERSIZE 256

class Packet;

class UDPClient
{
public:
	UDPClient(int port, std::string ipAddress);
	~UDPClient();
	void receivePackets();
	void sendPackets(Packet& packet);
	void virtualConnection();
	void virtualDisconnection();

	static int myID;

private:
	int port;
	WSADATA wsaData;
	struct sockaddr_in serverInfo;
	struct addrinfo *resultAddr;
	int sizeServerInfo;

	SOCKET connectionSocket;

	bool asyncSocket;

	// Methods
	void startWinsock();
	SOCKET createSocket();
	void updateServerInfo(std::string ipAddress);
	void updateClientInfo();
};

