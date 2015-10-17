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
#include "Projectile.h"
#include <iostream>

Player* PacketController::localPlayer = NULL;
std::vector<Player*>* PacketController::onlinePlayers = NULL;
Game* PacketController::game = NULL;
UDPClient* PacketController::udpClient = NULL;
GUI* PacketController::gui = NULL;

glm::vec3 PacketController::playerLastPosition = glm::vec3(0, 0, 0);

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
			if (xid == 0 && dataSize == 11 * sizeof(short))	// Creation of player
			{
				short playerClientId = data[0];

				if (playerClientId == UDPClient::myID)
					PacketController::game->createOnlinePlayer(data, true);
				else
					PacketController::game->createOnlinePlayer(data, false);
			}
			else if (xid == 1)	// Creation of Monster
			{
				int numberOfShort = dataSize / sizeof(short);

				for (int i = 0; i < numberOfShort; i+=8)
					PacketController::game->createMonster(data+i);
			}
			break;

		// UPDATE PLAYERS ATTRIBUTES/POSITION
		case 1:
			if (xid == 0 && (dataSize % (8*sizeof(short))) == 0) // Refresh player attributes
			{
				int numberOfShort = dataSize / sizeof(short);

				for (int i = 0; i < numberOfShort; i += 8)
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
	switch (id)
	{
	// CONNECTION
	case 0:
		if (data[0] == 1 && dataSize == sizeof(int))
		{
			// Connection ID back
			UDPClient::myID = xid;
			std::cout << UDPClient::myID << std::endl;
		}
		break;
	// ATTACK COLLISION
	case 4:
		if (dataSize == 3 * sizeof(int))
		{
			int hurtMonsterId = data[0];
			int projectileToBeDestroyed = data[1];
			int attack = data[2];

			PacketController::game->getMonsterOfId(hurtMonsterId)->doDamage(attack);
			PacketController::game->destroyProjectileOfId(projectileToBeDestroyed);
		}
		break;
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
	switch (id)
	{
	// CREATE NEW PROJECTILE (ATTACK)
	case 3:
		Player* player = PacketController::getPlayerOfClient(xid);
		if (player != NULL && dataSize == sizeof(glm::vec4))
		{
			player->getRangeAttack()->createProjectile(glm::vec3(data[0].x, data[0].y, data[0].z), (int)data[0].w);
		}
		break;
	}
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
	switch (id)
	{
	// UPDATE PLAYERS ATTRIBUTES/POSITION
	case 1:
		if (xid == 1 && (dataSize % (sizeof(glm::vec3) + sizeof(short)) == 0))	// Refresh Player Position
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
				{
					if (targetMonster->shouldRender())
						targetMonster->startMovementTo(glm::vec3(data[i].x, data[i].y, data[i].z));
					else
					{
						targetMonster->getTransform()->translate(data[i].x, data[i].y, data[i].z);
						targetMonster->setShouldRender(true);
					}
				}
			}
		}
		break;
	}
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
	if (playerLastPosition.x != playerPosition.x || playerLastPosition.y != playerPosition.y)
	{
		udpClient->sendPackets(Packet(playerPosition, 0, UDPClient::myID));
		playerLastPosition = playerPosition;
	}
}

void PacketController::sendAttackToServer(glm::vec3 attackDirection)
{
	udpClient->sendPackets(Packet(attackDirection, 1, UDPClient::myID));
}

void PacketController::sendAttackCollisionToServer(int monsterId, int attackId)
{
	int attackCollisionInformation[3];
	attackCollisionInformation[0] = monsterId;
	attackCollisionInformation[1] = attackId;
	attackCollisionInformation[2] = 20;	// TO DO: damage

	udpClient->sendPackets(Packet(attackCollisionInformation, 3, 2, UDPClient::myID));
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