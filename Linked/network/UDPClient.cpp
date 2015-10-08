#include "UDPClient.h"
#include "Packet.h"
#include "ClientPacket.h"
#include "../PacketController.h"
#include "../Game.h"
#include <iostream>

//#define DEBUG

using namespace std;

int UDPClient::myID = -1;

UDPClient::UDPClient(int port, string ipAddress)
{
	this->port = port;
	sizeServerInfo = sizeof(serverInfo);
	startWinsock();
	asyncSocket = true;
	connectionSocket = createSocket();
	updateServerInfo(ipAddress);
}


UDPClient::~UDPClient()
{
}

void UDPClient::receivePackets()
{
	char buffer[RECVBUFFERSIZE] = { 0 };
	if (recvfrom(connectionSocket, buffer, RECVBUFFERSIZE, 0, (struct sockaddr *) resultAddr->ai_addr, &sizeServerInfo) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error != WSAEWOULDBLOCK)
		{
			#ifdef DEBUG
			cerr << "recvfrom() failed with error code : " << WSAGetLastError() << endl;
			#endif
		}
		return;
	}
	ClientPacket *cp = new ClientPacket(buffer);
	PacketController::dispatch(cp);
	delete cp;
	#ifdef DEBUG
	cout << "Servidor: " << ((int*)buffer)[0] << endl;
	#endif
}


void UDPClient::sendPackets(Packet& packet)
{
	if (sendto(connectionSocket, packet.getData(), packet.getDataLength(), 0, (struct sockaddr *) resultAddr->ai_addr, sizeServerInfo) == SOCKET_ERROR)
	{
		#ifdef DEBUG
		cout << "sendto() failed with error code : " << WSAGetLastError() << endl;
		#endif
		return;
	}
}

void UDPClient::startWinsock()
{
	int status = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (status != 0)
	{
		#ifdef DEBUG
		cerr << "WSAStartup failed: " << status << endl;
		#endif
		exit(1);
	}
	#ifdef DEBUG
	cout << "WSAStartup inicializado." << endl;
	#endif
}

SOCKET UDPClient::createSocket()
{
	SOCKET dgramSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (dgramSocket == SOCKET_ERROR)
	{
		#ifdef DEBUG
		cerr << "Error creating socket: " << WSAGetLastError() << endl;
		#endif
		WSACleanup();
		exit(1);
	}
	u_long iMode = 1;
	if (asyncSocket)
		iMode = 1;
	else
		iMode = 0;
	ioctlsocket(dgramSocket, FIONBIO, &iMode);

	#ifdef DEBUG
	cout << "Conectado a :" << dgramSocket << endl;
	#endif
	return dgramSocket;
}

void UDPClient::updateServerInfo(string ipAddress)
{
	int status;
	std::string strPort = std::to_string(port);
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	status = getaddrinfo(ipAddress.c_str(), strPort.c_str(), &hints, &result);
	resultAddr = result;
	if (status != 0)
	{
	#ifdef DEBUG
		cerr << "getaddrinfo failed : " << status << endl;
	#endif

		WSACleanup();
		exit(1);
	}
	#ifdef DEBUG
	cout << "DNS realizado" << endl;
	#endif
}

void UDPClient::virtualConnection()
{
	Packet virtualConnection(1, 0, 0);
	sendPackets(virtualConnection);
}

void UDPClient::virtualDisconnection()
{
	Packet virtualDisconnection(2, 0, 0);
	sendPackets(virtualDisconnection);
}