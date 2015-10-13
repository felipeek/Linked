#include "Packet.h"
#include <iostream>

using namespace std;

void Packet::fillPacketType(int type)
{
	short T_uByte = type;
	*buffer = *((char*)&T_uByte);
	*(buffer + 1) = *((char*)&T_uByte + 1);
}

void Packet::fillPacketDataID(int id)
{
	*(buffer + PACKET_TYPE_SIZE) = *((char*)&id);
	*(buffer + 1 + PACKET_TYPE_SIZE) = *((char*)&id + 1);
	*(buffer + 2 + PACKET_TYPE_SIZE) = *((char*)&id + 2);
	*(buffer + 3 + PACKET_TYPE_SIZE) = *((char*)&id + 3);
}

void Packet::fillPacketDataXID(int xid)
{
	*(buffer + PACKET_TYPE_SIZE + PACKET_ID_SIZE) = *((char*)&xid);
	*(buffer + 1 + PACKET_TYPE_SIZE + PACKET_ID_SIZE) = *((char*)&xid + 1);
	*(buffer + 2 + PACKET_TYPE_SIZE + PACKET_ID_SIZE) = *((char*)&xid + 2);
	*(buffer + 3 + PACKET_TYPE_SIZE + PACKET_ID_SIZE) = *((char*)&xid + 3);
}

template<typename T>
void Packet::fillDataBuffers(int sizeOfType, int sizeData, int typeIndex, T data)
{
	multiplePacketSetup(typeIndex, sizeOfType*sizeData);

	// Fill data
	for (int i = 0; i < sizeData*sizeOfType; i += sizeOfType)
	{
		for (int j = 0; j < sizeOfType; j++)
		{
			buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i + j] = *((char*)data + j + i);
		}
	}
}

Packet::Packet(char* bufferByte, int size, int ID, int xID) : Packet(ID, xID)
{
	fillDataBuffers(sizeof(char), size, P_SINGLE_BYTE + size - 1, bufferByte);
}

Packet::Packet(char uByte, int ID, int xID) : Packet(ID, xID)
{
	uniquePacketSetup(P_SINGLE_BYTE, sizeof(char));

	// Fill data
	buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE] = uByte;
}

Packet::Packet(short* bufferShort, int size, int ID, int xID) : Packet(ID, xID)
{
	fillDataBuffers(sizeof(short), size, P_SINGLE_SHORT + size - 1, bufferShort);
}

Packet::Packet(short uShort, int ID, int xID) : Packet(ID, xID)
{
	uniquePacketSetup(P_SINGLE_SHORT, sizeof(short));

	// Fill data
	for (int i = 0; i < sizeof(short); i++)
	{
		buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] = *((char*)&uShort + i);
	}
}

Packet::Packet(int* bufferInt, int size, int ID, int xID) : Packet(ID, xID)
{
	fillDataBuffers(sizeof(int), size, P_SINGLE_INTEGER + size - 1, bufferInt);
}

Packet::Packet(int uInt, int ID, int xID) : Packet(ID, xID)
{
	uniquePacketSetup(P_SINGLE_INTEGER, sizeof(int));

	// Fill data
	for (int i = 0; i < sizeof(int); i++)
	{
		buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] = *((char*)&uInt + i);
	}
}

Packet::Packet(float* bufferFloat, int size, int ID, int xID) : Packet(ID, xID)
{
	fillDataBuffers(sizeof(float), size, P_SINGLE_FLOAT + size - 1, bufferFloat);
}

Packet::Packet(float uFloat, int ID, int xID) : Packet(ID, xID)
{
	uniquePacketSetup(P_SINGLE_FLOAT, sizeof(float));

	// Fill data
	for (int i = 0; i < sizeof(float); i++)
	{
		buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] = *((char*)&uFloat + i);
	}
}

Packet::Packet(long* bufferLong, int size, int ID, int xID) : Packet(ID, xID)
{
	fillDataBuffers(sizeof(long), size, P_SINGLE_LONG + size - 1, bufferLong);
}

Packet::Packet(long uLong, int ID, int xID) : Packet(ID, xID)
{
	uniquePacketSetup(P_SINGLE_LONG, sizeof(long));

	// Fill data
	for (int i = 0; i < sizeof(long); i++)
	{
		buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] = *((char*)&uLong + i);
	}
}

Packet::Packet(double* bufferDouble, int size, int ID, int xID) : Packet(ID, xID)
{
	fillDataBuffers(sizeof(double), size, P_SINGLE_DOUBLE + size - 1, bufferDouble);
}

Packet::Packet(double uDouble, int ID, int xID) : Packet(ID, xID)
{
	uniquePacketSetup(P_SINGLE_DOUBLE, sizeof(double));

	// Fill data
	for (int i = 0; i < sizeof(double); i++)
	{
		buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] = *((char*)&uDouble + i);
	}
}

Packet::Packet(glm::vec4* bufferVector4f, int size, int ID, int xID) : Packet(ID, xID)
{
	multiplePacketSetup(P_SINGLE_VECTOR4F + size - 1, size*sizeof(float)*4);
	for (int i = 0, a = 0; i < size*sizeof(float)*4; i+=sizeof(float)*4, a++)
	{
		// For each vector
		float x = bufferVector4f[a].x;
		float y = bufferVector4f[a].y;
		float z = bufferVector4f[a].z;
		float w = bufferVector4f[a].w;

		// For each component x, y, z & w
		for (int k = 0, j = 0; k < sizeof(float); k++, j++)
		{
			buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i + k] = *((char*)&x + j);
		}
		for (int k = sizeof(float), j = 0; k < sizeof(float) * 2; k++, j++)
		{																						    
			buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i + k] = *((char*)&y + j);
		}
		for (int k = sizeof(float) * 2, j = 0; k < sizeof(float) * 3; k++, j++)
		{
			buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i + k] = *((char*)&z + j);
		}
		for (int k = sizeof(float) * 3, j = 0; k < sizeof(float) * 4; k++, j++)
		{																						    
			buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i + k] = *((char*)&w + j);
		}
	}
}

Packet::Packet(glm::vec4 uVector4f, int ID, int xID) : Packet(ID, xID)
{
	uniquePacketSetup(P_SINGLE_VECTOR4F, sizeof(float)*4);

	float x = uVector4f.x;
	float y = uVector4f.y;
	float z = uVector4f.z;
	float w = uVector4f.w;

	// Fill data
	for (int i = 0; i < sizeof(float); i++)
	{
		buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] = *((char*)&x + i);
	}
	for (int i = sizeof(float), j = 0; i < sizeof(float) * 2; i++, j++)
	{
		buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] = *((char*)&y + j);
	}
	for (int i = sizeof(float) * 2, j = 0; i < sizeof(float) * 3; i++, j++)
	{
		buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] = *((char*)&z + j);
	}
	for (int i = sizeof(float) * 3, j = 0; i < sizeof(float) * 4; i++, j++)
	{
		buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] = *((char*)&w + j);
	}

}

Packet::Packet(glm::vec3* bufferVector3f, int size, int ID, int xID) : Packet(ID, xID)
{
	multiplePacketSetup(P_SINGLE_VECTOR3F + size - 1, size*sizeof(float) * 3);
	for (int i = 0, a = 0; i < size*sizeof(float) * 3; i += sizeof(float) * 3, a++)
	{
		// For each vector
		float x = bufferVector3f[a].x;
		float y = bufferVector3f[a].y;
		float z = bufferVector3f[a].z;

		// For each component x, y, z & w
		for (int k = 0, j = 0; k < sizeof(float); k++, j++)
		{
			buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i + k] = *((char*)&x + j);
		}
		for (int k = sizeof(float), j = 0; k < sizeof(float) * 2; k++, j++)
		{
			buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i + k] = *((char*)&y + j);
		}
		for (int k = sizeof(float) * 2, j = 0; k < sizeof(float) * 3; k++, j++)
		{
			buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i + k] = *((char*)&z + j);
		}
	}
}

Packet::Packet(glm::vec3* bufferVector3f,short* bufferShort, int size, int ID, int xID) : Packet(ID, xID)
{
	multiplePacketSetup(P_MONSTERS, size*sizeof(float) * 3 + size*sizeof(short));
	for (int i = 0, a = 0; i < size*sizeof(float) * 3 + size * sizeof(short); i += sizeof(float) * 3 + sizeof(short), a++)
	{
		// For each vector
		float x = bufferVector3f[a].x;
		float y = bufferVector3f[a].y;
		float z = bufferVector3f[a].z;

		// For each component x, y, z & w
		for (int k = 0, j = 0; k < sizeof(float); k++, j++)
		{
			buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i + k] = *((char*)&x + j);
		}
		for (int k = sizeof(float), j = 0; k < sizeof(float) * 2; k++, j++)
		{
			buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i + k] = *((char*)&y + j);
		}
		for (int k = sizeof(float) * 2, j = 0; k < sizeof(float) * 3; k++, j++)
		{
			buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i + k] = *((char*)&z + j);
		}
		buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i + sizeof(float) * 3] = ((char*)bufferShort)[sizeof(short)*a];
		buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i + sizeof(float) * 3 + 1] = ((char*)bufferShort)[sizeof(short)*a + 1];
	}
}


Packet::Packet(glm::vec3 uVector3f, int ID, int xID) : Packet(ID, xID)
{
	uniquePacketSetup(P_SINGLE_VECTOR3F, sizeof(float) * 3);

	float x = uVector3f.x;
	float y = uVector3f.y;
	float z = uVector3f.z;

	// Fill data
	for (int i = 0; i < sizeof(float); i++)
	{
		buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] = *((char*)&x + i);
	}
	for (int i = sizeof(float), j = 0; i < sizeof(float) * 2; i++, j++)
	{
		buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] = *((char*)&y + j);
	}
	for (int i = sizeof(float) * 2, j = 0; i < sizeof(float) * 3; i++, j++)
	{
		buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] = *((char*)&z + j);
	}
}

Packet::Packet(glm::vec2* bufferVector2f, int size, int ID, int xID) : Packet(ID, xID)
{
	multiplePacketSetup(P_SINGLE_VECTOR2F + size - 1, size*sizeof(float) * 2);
	for (int i = 0, a = 0; i < size*sizeof(float) * 2; i += sizeof(float) * 2, a++)
	{
		// For each vector
		float x = bufferVector2f[a].x;
		float y = bufferVector2f[a].y;

		// For each component x, y, z & w
		for (int k = 0, j = 0; k < sizeof(float); k++, j++)
		{
			buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i + k] = *((char*)&x + j);
		}
		for (int k = sizeof(float), j = 0; k < sizeof(float) * 2; k++, j++)
		{
			buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i + k] = *((char*)&y + j);
		}
	}
}

Packet::Packet(glm::vec2 uVector2f, int ID, int xID) : Packet(ID, xID)
{
	uniquePacketSetup(P_SINGLE_VECTOR2F, sizeof(float) * 2);

	float x = uVector2f.x;
	float y = uVector2f.y;

	// Fill data
	for (int i = 0; i < sizeof(float); i++)
	{
		buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] = *((char*)&x + i);
	}
	for (int i = sizeof(float), j = 0; i < sizeof(float) * 2; i++, j++)
	{
		buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] = *((char*)&y + j);
	}
}

Packet::Packet(int id, int xid)
{
	this->ID = id;
	this->xID = xid;

	switch (id)
	{
	case 0:
		// PING ID
		uniquePacketSetup(P_PING, 0);
		break;
	case 1:
		// PONG ID
		uniquePacketSetup(P_PONG, 0);
		break;
	}
}


Packet::Packet(std::string message, int ID) : Packet(ID, message.length())
{
	uniquePacketSetup(P_MSG, message.length());

	// Fill data
	for (int i = 0; i < message.length(); i++)
		buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] = (char)message[i];

}

void Packet::uniquePacketSetup(int type, int sizeOfType)
{
	sizeData = PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + sizeOfType;	// Single Double

	// Allocate buffer for packet
	buffer = new char[sizeData];

	// Fill packet type
	fillPacketType(type);
	fillPacketDataID(ID);
	fillPacketDataXID(xID);
}

void Packet::multiplePacketSetup(int type, int size)
{
	sizeData = PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + size;	// Single Double

	// Allocate buffer for packet
	buffer = new char[sizeData];

	// Fill packet type
	fillPacketType(type);
	fillPacketDataID(ID);
	fillPacketDataXID(xID);
}

void Packet::printPacket()
{
	cout << endl << "Single Packet:" << endl;
	cout << "Type: " << *(short*)buffer << endl;
	cout << "ID: " << *(int*)&buffer[PACKET_TYPE_SIZE] << endl;
	cout << "XID: " << *(int*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE] << endl;

	int type = *(short*)&buffer[0];

	switch (type)
	{
	case P_SINGLE_BYTE:
		cout << (int)*(char*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE] << endl;
		break;
	case P_SINGLE_SHORT:
		cout << *(short*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE] << endl;
		break;
	case P_SINGLE_INTEGER:
		cout << *(int*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE] << endl;
		break;
	case P_SINGLE_FLOAT:
		cout << *(float*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE] << endl;
		break;
	case P_SINGLE_LONG:
		cout << *(long*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE] << endl;
		break;
	case P_SINGLE_DOUBLE:
		cout << *(double*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE] << endl;
		break;
	case P_SINGLE_VECTOR4F:
		for (int i = 0; i < 4 * 4;i+=4)
			cout << *(float*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] << endl;
		break;
	case P_SINGLE_VECTOR3F:
		for (int i = 0; i < 4 * 3; i += 4)
			cout << *(float*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] << endl;
		break;
	case P_SINGLE_VECTOR2F:
		for (int i = 0; i < 4 * 2; i += 4)
			cout << *(float*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] << endl;
		break;
	case P_MSG:
		for (int i = 0; i < xID; i ++)
			cout << buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] << endl;
		break;
	default:
		
		if (type < P_SINGLE_SHORT && type > P_SINGLE_BYTE)
		{
			for (int i = P_SINGLE_BYTE; i <= type; i++)
				cout << (int)*(char*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] << endl;
		}
		else if (type >= P_SINGLE_SHORT && type < P_SINGLE_INTEGER)
		{
			for (int i = 0; i <= (type - P_SINGLE_SHORT)*sizeof(short); i += sizeof(short))
				cout << *(short*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] << endl;
		}
		else if (type >= P_SINGLE_INTEGER && type < P_SINGLE_FLOAT)
		{
			for (int i = 0; i <= (type - P_SINGLE_INTEGER) * sizeof(int); i += sizeof(int))
				cout << *(int*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] << endl;
		}
		else if (type >= P_SINGLE_FLOAT && type < P_SINGLE_LONG)
		{
			for (int i = 0; i <= (type - P_SINGLE_FLOAT) * sizeof(float); i += sizeof(float))
				cout << *(float*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] << endl;
		}
		else if (type >= P_SINGLE_LONG && type < P_SINGLE_DOUBLE)
		{
			for (int i = 0; i <= (type - P_SINGLE_LONG) * sizeof(long); i += sizeof(long))
				cout << *(long*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] << endl;
		}
		else if (type >= P_SINGLE_DOUBLE && type < P_SINGLE_VECTOR4F)
		{
			for (int i = 0; i <= (type - P_SINGLE_DOUBLE) * sizeof(double); i += sizeof(double))
				cout << *(double*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + i] << endl;
		}
		else if (type >= P_SINGLE_VECTOR4F && type < P_SINGLE_VECTOR3F)
		{
			for (int i = 0, j=0; i <= type - P_SINGLE_VECTOR4F; i++, j+=sizeof(float)*4)
			{
				cout << "x: " << *(float*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + j+0] << endl;
				cout << "y: " << *(float*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + j+4] << endl;
				cout << "z: " << *(float*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + j+8] << endl;
				cout << "w: " << *(float*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + j+12] << endl;
				cout << endl;
			}
		}
		else if (type >= P_SINGLE_VECTOR3F && type < P_SINGLE_VECTOR2F)
		{
			for (int i = 0, j = 0; i <= type - P_SINGLE_VECTOR3F; i++, j += sizeof(float) * 3)
			{
				cout << "x: " << *(float*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + j + 0] << endl;
				cout << "y: " << *(float*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + j + 4] << endl;
				cout << "z: " << *(float*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + j + 8] << endl;
				cout << endl;
			}
		}
		else if (type >= P_SINGLE_VECTOR2F && type < P_SINGLE_VECTOR4I)
		{
			for (int i = 0, j = 0; i <= type - P_SINGLE_VECTOR2F; i++, j += sizeof(float) * 2)
			{
				cout << "x: " << *(float*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + j + 0] << endl;
				cout << "y: " << *(float*)&buffer[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE + j + 4] << endl;
				cout << endl;
			}
		}
	}
}

char* Packet::getData()
{
	return this->buffer;
}

int Packet::getDataLength()
{
	return sizeData;
}

Packet::~Packet()
{
	delete[] this->buffer;
	//cout << "delete packet" << endl;
}
