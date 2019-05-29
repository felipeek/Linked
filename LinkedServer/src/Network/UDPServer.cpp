#include "UDPServer.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include "PacketController.h"
#include "Common.h"
#include <iostream>

using namespace std;

#define MAXLINE 1024

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

UDPServer::UDPServer(int port)
{
    struct sockaddr_in servaddr;

    // Creating socket file descriptor 
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    { 
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(port);

    // Bind the socket with the server address 
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }
}

UDPServer::~UDPServer()
{

}

void UDPServer::receivePackets()
{
    struct sockaddr_in clientInfo;
    memset(&clientInfo, 0, sizeof(clientInfo));
    char buffer[MAXLINE]; 
    socklen_t len, n; 
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_DONTWAIT, ( struct sockaddr *) &clientInfo, 
                &len);

    if (n == -1)
    {
        // nothing to receive
        if (errno == EWOULDBLOCK)
            return;
        cout << "receivePackets: error: " << strerror(errno) << endl;
        return;
    }
    if (n == 0)
        return;

    buffer[n] = '\0'; 
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

void UDPServer::sendPackets(Packet& packet, struct sockaddr_in& client)
{
    int status = sendto(sockfd, (const char *)packet.getData(), packet.getDataLength(),  
        0, (struct sockaddr*)&client, sizeof(struct sockaddr_in)); 
	if (status == -1)
	{
		#ifdef DEBUG
		//cerr << "Error sendto: " << WSAGetLastError() << endl;
		#endif
	}

    return;
}

std::vector <ClientInfo*>* UDPServer::getClients()
{
	return &clientsInfo;
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
	return -1;
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

int UDPServer::verifyClient(struct sockaddr_in* client)
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
	return 0;
}

double UDPServer::getPing()
{
    return 0.0;
}