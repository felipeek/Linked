#pragma once

#include "glm\glm.hpp"
#include "UDPServer.h"

class ClientPacket
{
public:
	ClientPacket(char* rawPacket, int senderID, struct sockaddr_in* clientInfo);
	~ClientPacket();
	void decodePacket(char* rawPacket);

	short getType();
	int getID();
	int getXID();
	int getDataSize();
	void* getData();
	int getSenderID();
	struct sockaddr_in* getClientInfo();
	
private:
	char* rawPacket;
	void printPacket();

	// Data
	short type;
	int ID;
	int xID;
	int sizeData;
	int senderID;

	void* data;
	struct sockaddr_in* clientInfo;
	
};

