#include "UDPClient.h"
#include "Packet.h"
#include "ClientPacket.h"
#include "../PacketController.h"
#include "../Game.h"
#include <iostream>

//#define DEBUG

using namespace std;

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
	char charArray[3] = { 1, 2, 3 };
	short shortArray[3] = { 1, 2, 3 };
	int intArray[3] = { 1, 2, 3 };
	float floatArray[3] = { 1, 2, 3 };
	long longArray[3] = { 1, 2, 3 };
	double doubleArray[3] = { 1, 2, 3 };
	glm::vec3 arranjo[2] = { glm::vec3(1, 2, 3), glm::vec3(5, 6, 7) };
	glm::vec4 arranjo1[2] = { glm::vec4(1.5f, 2.5f, 3.5f,7.5f), glm::vec4(5.5f, 6.5f, 7.5f,8.5f) };
	glm::vec2 arranjo2[2] = { glm::vec2(1, 3), glm::vec2(5, 7) };

	Packet virtualConnection(arranjo, 2, 0, 0);
	Packet virtualConnection1(arranjo1, 2, 0, 0);
	Packet virtualConnection2(arranjo2, 2, 0, 0);

	Packet virtualConnection3(charArray, 3, 7, 0);
	Packet virtualConnection4(shortArray, 3, 7, 0);
	Packet virtualConnection5(intArray, 3, 7, 0);
	Packet virtualConnection6(floatArray, 3, 7, 0);
	Packet virtualConnection7(longArray, 3, 7, 0);
	Packet virtualConnection8(doubleArray, 3, 7, 0);


	sendPackets(virtualConnection);
	//sendPackets(virtualConnection1);
	//sendPackets(virtualConnection2);
	//
	//sendPackets(virtualConnection3);
	//sendPackets(virtualConnection4);
	//sendPackets(virtualConnection5);
	//sendPackets(virtualConnection6);
	//sendPackets(virtualConnection7);
	//sendPackets(virtualConnection8);

}