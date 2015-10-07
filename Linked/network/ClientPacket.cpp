#include "ClientPacket.h"
#include "Packet.h"
#include <iostream>

using namespace std;

ClientPacket::ClientPacket(char* rawPacket)
{
	this->rawPacket = rawPacket;
	decodePacket(rawPacket);
}


ClientPacket::~ClientPacket()
{
}

void ClientPacket::decodePacket(char* rawPacket)
{
	// Decode Header
	type = *(short*)rawPacket;
	ID = *(int*)&rawPacket[PACKET_TYPE_SIZE];
	xID = *(int*)&rawPacket[PACKET_TYPE_SIZE + PACKET_ID_SIZE];

	// Decode Data

	int sizeData = -1;
	switch (type)
	{
	case P_SINGLE_BYTE:
		sizeData = sizeof(char);
		data = new char[sizeData]; break;
	case P_SINGLE_SHORT:
		sizeData = sizeof(short);
		data = new short[sizeData]; break;
	case P_SINGLE_INTEGER:
		sizeData = sizeof(int);
		data = new int[sizeData]; break;
	case P_SINGLE_FLOAT:
		sizeData = sizeof(float);
		data = new float[sizeData]; break;
	case P_SINGLE_LONG:
		sizeData = sizeof(long);
		data = new long[sizeData]; break;
	case P_SINGLE_DOUBLE:
		sizeData = sizeof(double);
		data = new double[sizeData]; break;
	case P_SINGLE_VECTOR4F:
		sizeData = sizeof(float) * 4;
		data = new glm::vec4[sizeData]; break;
	case P_SINGLE_VECTOR3F:
		sizeData = sizeof(float) * 3;
		data = new glm::vec3[sizeData]; break;
	case P_SINGLE_VECTOR2F:
		sizeData = sizeof(float) * 2;
		data = new glm::vec2[sizeData]; break;
	default:
		if (type < P_SINGLE_SHORT && type > P_SINGLE_BYTE)
		{
			sizeData = (type - P_SINGLE_BYTE + 1) * sizeof(char);
			data = new char[sizeData];
		}
		else if (type >= P_SINGLE_SHORT && type < P_SINGLE_INTEGER)
		{
			sizeData = (type - P_SINGLE_SHORT + 1) * sizeof(short);
			data = new short[sizeData];
		}
		else if (type >= P_SINGLE_INTEGER && type < P_SINGLE_FLOAT)
		{
			sizeData = (type - P_SINGLE_INTEGER + 1) * sizeof(int);
			data = new int[sizeData];
		}
		else if (type >= P_SINGLE_FLOAT && type < P_SINGLE_LONG)
		{
			sizeData = (type - P_SINGLE_FLOAT + 1) * sizeof(float);
			data = new float[sizeData];
		}
		else if (type >= P_SINGLE_LONG && type < P_SINGLE_DOUBLE)
		{
			sizeData = (type - P_SINGLE_LONG + 1) * sizeof(long);
			data = new long[sizeData];
		}
		else if (type >= P_SINGLE_DOUBLE && type < P_SINGLE_VECTOR4F)
		{
			sizeData = (type - P_SINGLE_DOUBLE + 1) * sizeof(double);
			data = new double[sizeData];
		}
		else if (type >= P_SINGLE_VECTOR4F && type < P_SINGLE_VECTOR3F)
		{
			sizeData = (type - P_SINGLE_VECTOR4F + 1) * sizeof(float) * 4;
			data = new glm::vec4[sizeData];
		}
		else if (type >= P_SINGLE_VECTOR3F && type < P_SINGLE_VECTOR2F)
		{
			sizeData = (type - P_SINGLE_VECTOR3F + 1) * sizeof(float) * 3;
			data = new glm::vec3[sizeData];
		}
		else if (type >= P_SINGLE_VECTOR2F && type < P_SINGLE_VECTOR4I)
		{
			sizeData = (type - P_SINGLE_VECTOR2F + 1) * sizeof(float) * 2;
			data = new glm::vec2[sizeData];
		}
	}
	std::memcpy(data, &rawPacket[PACKET_TYPE_SIZE + PACKET_ID_SIZE + PACKET_XID_SIZE], sizeData);
}

#define DEBUG

#ifdef DEBUG
void ClientPacket::printPacket()
{

	switch (type)
	{
	case P_SINGLE_BYTE:
		cout << (int)((char*)data)[0] << endl;
		break;
	case P_SINGLE_SHORT:
		cout << ((short*)data)[0] << endl;
		break;
	case P_SINGLE_INTEGER:
		cout << ((int*)data)[0] << endl;
		break;
	case P_SINGLE_FLOAT:
		cout << ((float*)data)[0] << endl;
		break;
	case P_SINGLE_LONG:
		cout << ((long*)data)[0] << endl;
		break;
	case P_SINGLE_DOUBLE:
		cout << ((double*)data)[0] << endl;
		break;
	case P_SINGLE_VECTOR4F:
		for (int i = 0; i < 4; i++)
			cout << ((float*)data)[i] << endl;
		break;
	case P_SINGLE_VECTOR3F:
		for (int i = 0; i < 3; i++)
			cout << ((float*)data)[i] << endl;
		break;
	case P_SINGLE_VECTOR2F:
		for (int i = 0; i < 2; i++)
			cout << ((float*)data)[i] << endl;
		break;
	default:

		if (type < P_SINGLE_SHORT && type > P_SINGLE_BYTE)
		{
			for (int i = P_SINGLE_BYTE; i <= type; i++)
				cout << (int)((char*)data)[i] << endl;
		}
		else if (type >= P_SINGLE_SHORT && type < P_SINGLE_INTEGER)
		{
			for (int i = 0; i <= (type - P_SINGLE_SHORT); i++)
				cout << ((short*)data)[i] << endl;
		}
		else if (type >= P_SINGLE_INTEGER && type < P_SINGLE_FLOAT)
		{
			for (int i = 0; i <= (type - P_SINGLE_INTEGER); i++)
				cout << ((int*)data)[i] << endl;
		}
		else if (type >= P_SINGLE_FLOAT && type < P_SINGLE_LONG)
		{
			for (int i = 0; i <= (type - P_SINGLE_FLOAT); i++)
				cout << ((float*)data)[i] << endl;
		}
		else if (type >= P_SINGLE_LONG && type < P_SINGLE_DOUBLE)
		{
			for (int i = 0; i <= (type - P_SINGLE_LONG); i++)
				cout << ((long*)data)[i] << endl;
		}
		else if (type >= P_SINGLE_DOUBLE && type < P_SINGLE_VECTOR4F)
		{
			for (int i = 0; i <= (type - P_SINGLE_DOUBLE); i++)
				cout << ((double*)data)[i] << endl;
		}
		else if (type >= P_SINGLE_VECTOR4F && type < P_SINGLE_VECTOR3F)
		{
			for (int i = 0; i <= type - P_SINGLE_VECTOR4F; i++)
			{
				cout << "x: " << ((float*)data)[4 * i] << endl;
				cout << "y: " << ((float*)data)[1 + 4 * i] << endl;
				cout << "z: " << ((float*)data)[2 + 4 * i] << endl;
				cout << "w: " << ((float*)data)[3 + 4 * i] << endl;
				cout << endl;
			}
		}
		else if (type >= P_SINGLE_VECTOR3F && type < P_SINGLE_VECTOR2F)
		{
			for (int i = 0; i <= type - P_SINGLE_VECTOR3F; i++)
			{
				cout << "x: " << ((float*)data)[3 * i] << endl;
				cout << "y: " << ((float*)data)[1 + 3 * i] << endl;
				cout << "z: " << ((float*)data)[2 + 3 * i] << endl;
				cout << endl;
			}
		}
		else if (type >= P_SINGLE_VECTOR2F && type < P_SINGLE_VECTOR4I)
		{
			for (int i = 0; i <= type - P_SINGLE_VECTOR2F; i++)
			{
				cout << "x: " << ((float*)data)[2 * i] << endl;
				cout << "y: " << ((float*)data)[1 + 2 * i] << endl;
				cout << endl;
			}
		}
	}
}
#endif