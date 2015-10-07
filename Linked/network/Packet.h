#pragma once
#include "glm\glm.hpp"
#include <string>

// Packet Initial positions

#define PACKET_TYPE_SIZE 2
#define PACKET_ID_SIZE 4
#define PACKET_XID_SIZE 4

// Packet type definitions

#define P_SINGLE_BYTE 0
#define P_SINGLE_SHORT 128
#define P_SINGLE_INTEGER 193
#define P_SINGLE_FLOAT 225
#define P_SINGLE_LONG 241
#define P_SINGLE_DOUBLE 257
#define P_SINGLE_VECTOR4F 273
#define P_SINGLE_VECTOR3F 277
#define P_SINGLE_VECTOR2F 281
#define P_SINGLE_VECTOR4I 285
#define P_SINGLE_VECTOR3I 289
#define P_SINGLE_VECTOR2I 293

#define P_PING 297
#define P_PONG 298

#define P_MSG 299

class Packet
{
public:
	Packet(int ID, int xID);

	Packet(char* bufferByte, int size, int ID, int xID);
	Packet(char uByte, int ID, int xID);

	Packet(short* bufferShort, int size, int ID, int xID);
	Packet(short uShort, int ID, int xID);

	Packet(int* bufferInt, int size, int ID, int xID);
	Packet(int uInt, int ID, int xID);

	Packet(float* bufferFloat, int size, int ID, int xID);
	Packet(float uFloat, int ID, int xID);

	Packet(long* bufferLong, int size, int ID, int xID);
	Packet(long uLong, int ID, int xID);

	Packet(double* bufferDouble, int size, int ID, int xID);
	Packet(double uDouble, int ID, int xID);

	Packet(glm::vec4* bufferVector4f, int size, int ID, int xID);
	Packet(glm::vec4 uVector4f, int ID, int xID);

	Packet(glm::vec3* bufferVector3f, int size, int ID, int xID);
	Packet(glm::vec3 uVector3f, int ID, int xID);

	Packet(glm::vec2* bufferVector2f, int size, int ID, int xID);
	Packet(glm::vec2 uVector2f, int ID, int xID);
	
	Packet(std::string message, int ID, int xID);

	// TODO: int vectors

	// TODO: ping packet

	~Packet();

	void printPacket();
	char* getData();
	int getDataLength();
private:
	char* buffer;
	int sizeData;
	int ID;
	int xID;

	void fillPacketType(int type);
	void fillPacketDataID(int id);
	void fillPacketDataXID(int xid);

	void uniquePacketSetup(int type, int sizeOfType);
	void multiplePacketSetup(int type, int size);

	template<typename T>
	void fillDataBuffers(int sizeOfType, int sizeData, int typeIndex, T data);
	
};

