#include "PacketController.h"
#include "network\ClientPacket.h"
#include "network\Packet.h"
#include "Player.h"
#include "network\UDPClient.h"
#include "Primitive.h"
#include "Game.h"
#include "Monster.h"
#include <iostream>

Player* PacketController::secondPlayer = NULL;
Player* PacketController::localPlayer = NULL;
Game* PacketController::game = NULL;
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
}

void PacketController::dispatchByteArray(int id, int xid, char* data, int dataSize)
{

}
void PacketController::dispatchShortArray(int id, int xid, short* data, int dataSize)
{
	switch (id)
	{
		// Second Player control
		case 0:
			if (xid == 1)	// Refresh Second Player Attributes
			{
				if (PacketController::secondPlayer != NULL && dataSize == 7 * sizeof(short))
				{
					short maxHp = data[0];
					short hp = data[1];
					short attack = data[2];
					short defense = data[3];
					short magPower = data[4];
					short speed = data[5];
					short aspd = data[6];
#ifdef MULTIPLAYER
					if (hp < PacketController::secondPlayer->getHp())
						PacketController::secondPlayer->receiveDamage();
					PacketController::secondPlayer->setTotalMaximumHp(maxHp);
					PacketController::secondPlayer->setHp(hp);
					PacketController::secondPlayer->setTotalAttack(attack);
					PacketController::secondPlayer->setTotalDefense(defense);
					PacketController::secondPlayer->setTotalMagicalPower(magPower);
					PacketController::secondPlayer->setTotalSpeed(speed);
					PacketController::secondPlayer->setTotalAttackSpeed(aspd);
#endif
				}
			}
			else if (xid == 2)
			{
				if (dataSize == 11 * sizeof(short))
				{
					PacketController::game->createOnlinePlayer(data, false);
				}
			}
			break;
		// Local Player Control
		case 1:
			if (xid == 0) // Creation of Local Player
			{
				if (dataSize == 11 * sizeof(short))
				{
					PacketController::game->createOnlinePlayer(data, true);
				}
			}
			else if (xid == 1)	// Refresh Local Player Attributes
			{
				if (PacketController::localPlayer != NULL && dataSize == 7 * sizeof(short))
				{
					short maxHp = data[0];
					short hp = data[1];
					short attack = data[2];
					short defense = data[3];
					short magPower = data[4];
					short speed = data[5];
					short aspd = data[6];
#ifdef MULTIPLAYER
					if (hp < PacketController::localPlayer->getHp())
						PacketController::localPlayer->receiveDamage();
					PacketController::localPlayer->setTotalMaximumHp(maxHp);
					PacketController::localPlayer->setHp(hp);
					PacketController::localPlayer->setTotalAttack(attack);
					PacketController::localPlayer->setTotalDefense(defense);
					PacketController::localPlayer->setTotalMagicalPower(magPower);
					PacketController::localPlayer->setTotalSpeed(speed);
					PacketController::localPlayer->setTotalAttackSpeed(aspd);
#endif
				}
			}
			break;
		// Monster Control
		default:
			if (xid == 0)	// Creation of Monster
			{
				if (dataSize == 7 * sizeof(short))
				{
					PacketController::game->createMonster(id, data);
				}
			}
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
#ifdef MULTIPLAYER
	switch (id)
	{
	// Second Player control
	case 0:
		if (xid == 0)	// Change Second Player Position
			if (PacketController::secondPlayer != NULL)
				secondPlayer->startMovementTo(data[0]);
		break;
	}
#endif
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
		// Monster Control
	default:
		if (xid == 1)	// Change Monster Position
		{
			int numberOfPositions = dataSize / (sizeof(glm::vec3) + sizeof(short));

			for (int i = 0; i < numberOfPositions; i++)
			{
				Monster* targetMonster = PacketController::game->getMonsterOfId(extraData[i]);
				if (targetMonster != NULL)
					targetMonster->startMovementTo(glm::vec3(data[i].x, data[i].y, data[i].z));
				//targetMonster->getTransform()->translate(data[i].y, data[i].z, data[i].w);
			}
		}
		break;
	}
#endif
}
void PacketController::update10()
{
	// Sends player position to server 10 times per second
	glm::vec3 playerPosition = PacketController::localPlayer->getTransform()->getPosition();
	udpClient->sendPackets(Packet(playerPosition, 0, UDPClient::myID));

#ifdef MULTIPLAYER
	// Send player attributes, if necessary
	if (PacketController::localPlayer->needToSendAttributesToServer())
		PacketController::updatePlayerBasicAttributes(PacketController::localPlayer);
#endif
}

void PacketController::updatePlayerBasicAttributes(Player* player)
{
#ifdef MULTIPLAYER
	short data[7];
	data[0] = PacketController::localPlayer->getTotalMaximumHp();
	data[1] = PacketController::localPlayer->getHp();
	data[2] = PacketController::localPlayer->getTotalAttack();
	data[3] = PacketController::localPlayer->getTotalDefense();
	data[4] = PacketController::localPlayer->getTotalMagicalPower();
	data[5] = PacketController::localPlayer->getTotalSpeed();
	data[6] = PacketController::localPlayer->getTotalAttackSpeed();
	udpClient->sendPackets(Packet((short*)data, 7, 0, UDPClient::myID));
#endif
}