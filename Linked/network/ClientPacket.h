#pragma once

#include "glm\glm.hpp"
//#define DEBUG

enum ClientPacketTypes{
	P_BYTE,
	P_SHORT,
	P_INTEGER,
	P_FLOAT,
	P_LONG,
	P_DOUBLE,
	P_VECTOR4F,
	P_VECTOR3F,
	P_VECTOR2F,
	P_MAX
};

class ClientPacket
{
public:
	ClientPacket(char* rawPacket);
	~ClientPacket();
	void decodePacket(char* rawPacket);

	short getType();
	int getID();
	int getXID();
	int getDataSize();
	void* getData(); 
	void* getExtraData();
private:

	// Data
	short type;
	int ID;
	int xID;
	int sizeData;
	void* data;
	void* extraData;

#ifdef DEBUG
	void printPacket();
#endif
};

