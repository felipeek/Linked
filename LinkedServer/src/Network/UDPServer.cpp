#include "UDPServer.h"
#include <iostream>
#include "LinkedTime.h"
#include "PacketController.h"

using namespace std;

//#define DEBUG

UDPServer::UDPServer(int port)
{
	this->port = port;
	sizeServerInfo = sizeof(serverInfo);
	sizeClientInfo = sizeof(clientInfo);
	startWinsock();
	asyncSocket = true;
	createSocket();
	initAndBindSocket();
}

UDPServer::~UDPServer()
{
}

void UDPServer::createSocket()
{
	//Create a socket
	if ((connectionSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
#ifdef DEBUG
		cerr << "Could not create socket : " << WSAGetLastError() << endl;
#else
		cerr << "Erro na criacao do socket de rede." << endl;
#endif
		WSACleanup();
		exit(1);
	}
	u_long iMode = 1;
	if (asyncSocket)
		iMode = 1;
	else
		iMode = 0;
	ioctlsocket(connectionSocket, FIONBIO, &iMode);
#ifdef DEBUG
	cout << "Socket created." << endl;
#endif
}

void UDPServer::initAndBindSocket()
{
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr.s_addr = INADDR_ANY;
	serverInfo.sin_port = htons(port);

	//Bind
	if (bind(connectionSocket, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) == SOCKET_ERROR)
	{
#ifdef DEBUG
		cerr <<"Bind failed with error code : " << WSAGetLastError() << endl;
#else
		cerr << "Erro ao fazer o bind do socket." << endl;
#endif
		closesocket(connectionSocket);
		WSACleanup();
		exit(1);
	}
#ifdef DEBUG
	cout << "Bind successful" << endl;
#endif
}

void UDPServer::startWinsock()
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

void UDPServer::receivePackets()
{
	char buffer[RECVBUFFERSIZE];
	int status = recvfrom(connectionSocket, buffer, RECVBUFFERSIZE, 0, (struct sockaddr*)&clientInfo, &sizeClientInfo);
	if (status == SOCKET_ERROR)
	{
		int ierr = WSAGetLastError();
		if (ierr == WSAEWOULDBLOCK) {  // currently no data available
			//Sleep(1);  // wait and try again
		}
		else
		{
			//#ifdef DEBUG
			cerr << "Error recvfrom :" << WSAGetLastError() << endl;
			//#endif
		}
		return;
	}
	else
	{
#ifdef DEBUG
		cout << inet_ntoa(clientInfo.sin_addr) << " (" << ntohs(clientInfo.sin_port) << "):";
		cout << (short)buffer[0] << endl;
#endif
		int id = verifyClient(&clientInfo);
		ClientPacket *cp = new ClientPacket(buffer, id, &clientInfo);
		if ((id != -1 || PacketController::isConnectionPacket(cp)) && !(id != -1 && PacketController::isConnectionPacket(cp)))
			PacketController::dispatch(cp);
		delete cp;
	}
}

void UDPServer::sendPackets(Packet& packet, struct sockaddr_in& client)
{
	int status = sendto(connectionSocket, packet.getData(), packet.getDataLength(), 0, (struct sockaddr*)&client, sizeClientInfo);
	if (status == SOCKET_ERROR)
	{
		#ifdef DEBUG
		cerr << "Error sendto: " << WSAGetLastError() << endl;
		#endif
	}

	last = LinkedTime::getTime();
}

int UDPServer::addToClients(struct sockaddr_in* client)
{
	bool clientExists = false;
	for (unsigned int i = 0; i < clientsInfo.size(); i++)
	{
		if (clientsInfo[i]->netInfo == *client)
		{
			clientExists = true;
			return clientsInfo[i]->id;
		}
	}
	if(!clientExists) 
	{
		/* v does not contain x */
		ClientInfo* ci = new ClientInfo(ClientInfo::currentId, *client);
		clientsInfo.push_back(ci);
		return ClientInfo::currentId;
	}
}

int UDPServer::clientExists(int id)
{
	for (unsigned int i = 0; i < clientsInfo.size(); i++)
	{
		if (clientsInfo[i]->id == id)
			return i;
	}
	return -1;
}

int UDPServer::verifyClient(sockaddr_in* client)
{
	bool clientExists = false;
	for (unsigned int i = 0; i < clientsInfo.size(); i++)
	{
		if (clientsInfo[i]->netInfo == *client)
		{
			clientExists = true;
			return clientsInfo[i]->id;
		}
	}
	if (!clientExists)
	{
		return -1;
	}
}

std::vector <ClientInfo*>* UDPServer::getClients()
{
	return &clientsInfo;
}

double UDPServer::getPing()
{
	return this->ping;
}

bool operator==(struct sockaddr_in l, struct sockaddr_in r)
{
	if (l.sin_port == r.sin_port)
		return true;
	else
		return false;
}
bool operator!=(struct sockaddr_in l, struct sockaddr_in r)
{
	if (l.sin_port != r.sin_port)
		return true;
	else
		return false;
}