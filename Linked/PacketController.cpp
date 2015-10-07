#include "PacketController.h"
#include "network\ClientPacket.h"
#include "network\Packet.h"
#include "Entity.h"
#include "network\UDPClient.h"

Entity* PacketController::secondPlayer = NULL;
UDPClient* PacketController::udpClient = NULL;

void PacketController::dispatch(ClientPacket* cp)
{
	short type = cp->getType();
	int size = cp->getDataSize();

	if (type >= P_SINGLE_BYTE && type < P_SINGLE_SHORT)
	{
		dispatchByteArray(cp->getID(), cp->getXID(), (char*)cp->getData(), cp->getDataSize());
	}
	else if (type >= P_SINGLE_SHORT && type < P_SINGLE_INTEGER)
	{
		dispatchShortArray(cp->getID(), cp->getXID(), (short*)cp->getData(), cp->getDataSize());
	}
	else if (type >= P_SINGLE_INTEGER && type < P_SINGLE_FLOAT)
	{
		dispatchIntArray(cp->getID(), cp->getXID(), (int*)cp->getData(), cp->getDataSize());
	}
	else if (type >= P_SINGLE_FLOAT && type < P_SINGLE_LONG)
	{
		dispatchFloatArray(cp->getID(), cp->getXID(), (float*)cp->getData(), cp->getDataSize());
	}
	else if (type >= P_SINGLE_LONG && type < P_SINGLE_DOUBLE)
	{
		dispatchLongArray(cp->getID(), cp->getXID(), (long*)cp->getData(), cp->getDataSize());
	}
	else if (type >= P_SINGLE_DOUBLE && type < P_SINGLE_VECTOR4F)
	{
		dispatchDoubleArray(cp->getID(), cp->getXID(), (double*)cp->getData(), cp->getDataSize());
	}
	else if (type >= P_SINGLE_VECTOR4F && type < P_SINGLE_VECTOR3F)
	{
		dispatchVec4fArray(cp->getID(), cp->getXID(), (glm::vec4*)cp->getData(), cp->getDataSize());
	}
	else if (type >= P_SINGLE_VECTOR3F && type < P_SINGLE_VECTOR2F)
	{
		dispatchVec3fArray(cp->getID(), cp->getXID(), (glm::vec3*)cp->getData(), cp->getDataSize());
	}
	else if (type >= P_SINGLE_VECTOR2F && type < P_SINGLE_VECTOR4I)
	{
		dispatchVec2fArray(cp->getID(), cp->getXID(), (glm::vec2*)cp->getData(), cp->getDataSize());
	}
}

void PacketController::dispatchByteArray(int id, int xid, char* data, int dataSize)
{

}
void PacketController::dispatchShortArray(int id, int xid, short* data, int dataSize)
{

}
void PacketController::dispatchIntArray(int id, int xid, int* data, int dataSize)
{

}
void PacketController::dispatchFloatArray(int id, int xid, float* data, int dataSize)
{

}
void PacketController::dispatchLongArray(int id, int xid, long* data, int dataSize)
{

}
void PacketController::dispatchDoubleArray(int id, int xid, double* data, int dataSize)
{

}
void PacketController::dispatchVec4fArray(int id, int xid, glm::vec4* data, int dataSize)
{

}
void PacketController::dispatchVec3fArray(int id, int xid, glm::vec3* data, int dataSize)
{
	switch (id)
	{
	// Second Player control
	case 0:
		if (xid == 0)
		{
			if (PacketController::secondPlayer != NULL)
			{
				secondPlayer->getTransform()->translate(data[0].x, data[0].y, data[0].z);
				udpClient->sendPackets(Packet(1, 0, 0));
			}
		}
		break;
	
	}
}
void PacketController::dispatchVec2fArray(int id, int xid, glm::vec2* data, int dataSize)
{

}