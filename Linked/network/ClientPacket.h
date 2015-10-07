#pragma once

#include "glm\glm.hpp"

class ClientPacket
{
public:
	ClientPacket(char* rawPacket);
	~ClientPacket();
	void decodePacket(char* rawPacket);
	/*private:*/
	char* rawPacket;
	void printPacket();

	// Data
	short type;
	int ID;
	int xID;

	void* data;

};

