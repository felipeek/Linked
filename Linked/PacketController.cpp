#include "PacketController.h"
#include "network\ClientPacket.h"
#include "network\Packet.h"
#include "Player.h"
#include "network\UDPClient.h"
#include "Primitive.h"
#include "Game.h"
#include "Monster.h"
#include "RangeAttack.h"
#include "GUI.h"
#include <iostream>

Player* PacketController::localPlayer = NULL;
std::vector<Player*>* PacketController::onlinePlayers = NULL;
Game* PacketController::game = NULL;
UDPClient* PacketController::udpClient = NULL;
GUI* PacketController::gui = NULL;

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
	else if (type == P_PING)
	{
		dispatchPing(cp->getID(), cp->getXID());
	}
	else if (type == P_PONG)
	{
		dispatchPong(cp->getID(), cp->getXID());
	}
	else if (type >= P_MONSTERS && type < END)
	{
		dispatchVec3fWithShortArray(cp->getID(), cp->getXID(), (glm::vec3*)cp->getData(), (short*)cp->getExtraData(), cp->getDataSize());
	}
	else if (type == P_MSG)
	{
		dispatchMsg(cp->getID(), cp->getXID(), (char*)cp->getData());
	}
}

void PacketController::dispatchByteArray(int id, int xid, char* data, int dataSize)
{

}
void PacketController::dispatchShortArray(int id, int xid, short* data, int dataSize)
{
	switch (id)
	{
		// CREATION/INITIALIZATION
		case 0:
			if (xid == 0)	// Creation of player
			{
				short playerClientId = data[0];

				if (playerClientId == UDPClient::myID)
					PacketController::game->createOnlinePlayer(data, true);
				else
					PacketController::game->createOnlinePlayer(data, false);
			}
			else if (xid == 1)
			{
				int numberOfShort = dataSize / sizeof(short);

				for (int i = 0; i < dataSize; i+=8)
					PacketController::game->createMonster(data+i);
			}
			break;

		// UPDATE PLAYERS ATTRIBUTES/POSITION
		case 1:
			if (xid == 0) // Refresh player attributes
			{
				int numberOfShort = dataSize / sizeof(short);

				for (int i = 0; i < dataSize; i += 8)
				{
					short playerClientId = (data + i)[0];
					Player* player = PacketController::getPlayerOfClient(playerClientId);
					if (player != NULL)
					{
						player->setTotalMaximumHp((data + i)[1]);
						player->setHp((data + i)[2]);
						player->setTotalAttack((data + i)[3]);
						player->setTotalDefense((data + i)[4]);
						player->setTotalMagicalPower((data + i)[5]);
						player->setTotalSpeed((data + i)[6]);
						player->setTotalAttackSpeed((data + i)[7]);
					}
				}
			}
			break;
	}
}
void PacketController::dispatchIntArray(int id, int xid, int* data, int dataSize)
{
	if (data[0] == 1 && id == 0 && dataSize == sizeof(int))
	{
		// Connection ID back
		UDPClient::myID = xid;
		std::cout << UDPClient::myID << std::endl;
	}
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
}
void PacketController::dispatchVec2fArray(int id, int xid, glm::vec2* data, int dataSize)
{

}
void PacketController::dispatchPing(int id, int xid)
{
	udpClient->sendPackets(Packet(ID_PONG, UDPClient::myID));
}
void PacketController::dispatchPong(int id, int xid)
{

}
void PacketController::dispatchVec3fWithShortArray(int id, int xid, glm::vec3* data, short* extraData, int dataSize)
{
#ifdef MULTIPLAYER
	switch (id)
	{
	// UPDATE PLAYERS ATTRIBUTES/POSITION
	case 1:
		if (xid == 1)	// Refresh Player Position
		{
			int numberOfPositions = dataSize / (sizeof(glm::vec3) + sizeof(short));

			for (int i = 0; i < numberOfPositions; i++)
			{
				if (extraData[i] != UDPClient::myID) // we don't want to refresh local Player position
				{
					Player* player = PacketController::getPlayerOfClient(extraData[i]);
					if (player != NULL)	player->startMovementTo(glm::vec3(data[i].x, data[i].y, data[i].z));
				}
			}
		}
		break;
		// UPDATE MONSTERS ATTRIBUTES/POSITION
	case 2:
		if (xid == 0)	// Refresh Monster Position
		{
			int numberOfPositions = dataSize / (sizeof(glm::vec3) + sizeof(short));

			for (int i = 0; i < numberOfPositions; i++)
			{
				Monster* targetMonster = PacketController::game->getMonsterOfId(extraData[i]);
				if (targetMonster != NULL)
					targetMonster->startMovementTo(glm::vec3(data[i].x, data[i].y, data[i].z));
					//targetMonster->getTransform()->translate(data[i].x, data[i].y, data[i].z);
			}
		}
		break;
	}
#endif
}
void PacketController::dispatchMsg(int id, int xid, char* data)
{
	// id = senderID	xid = msg size
	std::string msg(data);
	std::stringstream ss;

	//ss << PacketController::secondPlayer->getName() << ": " << msg;
	gui->setNextMessage(ss.str());
}

void PacketController::update10()
{
	// Sends player position to server 10 times per second
	glm::vec3 playerPosition = PacketController::localPlayer->getTransform()->getPosition();
	udpClient->sendPackets(Packet(playerPosition, 0, UDPClient::myID));
}

void PacketController::sendAttackToServer(glm::vec3 attackDirection)
{
	udpClient->sendPackets(Packet(attackDirection, 1, UDPClient::myID));
}

Player* PacketController::getPlayerOfClient(int clientId)
{
	if (PacketController::localPlayer->getClientId() == clientId)
		return PacketController::localPlayer;

	for (Player* player : *PacketController::onlinePlayers)
		if (player->getClientId() == clientId)
			return player;

	return NULL;
}