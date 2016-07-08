#include "PacketController.h"
#include "ClientPacket.h"
#include "network\Packet.h"
#include "network\UDPServer.h"
#include "LinkedTime.h"
#include "Player.h"
#include "Monster.h"
#include "Game.h"
#include "Projectile.h"
#include "Common.h"

#define SHOW_PACKETS_LOG 1

#define ADDRESS_IPV4_LENGHT 46

std::vector<Player*>* PacketController::players = NULL;
UDPServer* PacketController::udpServer = NULL;
std::vector<Monster*>* PacketController::monsters = NULL;
Game* PacketController::game = NULL;
std::set<int> PacketController::monstersThatAttacked = std::set<int>();
std::vector<int> PacketController::monsterDamages = std::vector<int>();
ClientPacket* PacketController::clientPacket = NULL;

bool PacketController::isConnectionPacket(ClientPacket* cp)
{
	if (cp->getType() >= P_SINGLE_INTEGER && cp->getType() < P_SINGLE_FLOAT)
		if (((int*)cp->getData())[0] == 1 && cp->getDataSize() == sizeof(int))
			if (cp->getID() == 0)
				return true;

	return false;
}

void PacketController::dispatch(ClientPacket* cp)
{
	short type = cp->getType();
	int size = cp->getDataSize();

	PacketController::clientPacket = cp;

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
	else if (type == P_MSG)
	{
		dispatchMsg(cp->getID(), cp->getXID(), (char*)cp->getData(), cp->getSenderID());
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
	switch (id)
	{
	// CONNECTION AND DISCONNECTION
	case 0:
		if (data[0] == 1 && dataSize == sizeof(int))
		{
			udpServer->addToClients(PacketController::clientPacket->getClientInfo());
			// Connection Packet
			char address[ADDRESS_IPV4_LENGHT] = { 0 };
			std::cout << "Client (" << inet_ntop(udpServer->getClients()->back()->netInfo.sin_family, &udpServer->getClients()->back()->netInfo.sin_addr, address, ADDRESS_IPV4_LENGHT)
				<< ":" << udpServer->getClients()->back()->netInfo.sin_port << ") " <<
				"connected with ID " << udpServer->getClients()->back()->id << std::endl;
			// Send their ID back
			udpServer->sendPackets(Packet(1, 0, udpServer->getClients()->back()->id), udpServer->getClients()->back()->netInfo);

			// Send essential game information for client
			PacketController::sendEssentialGameInformationToClient(udpServer->getClients()->back());
		}
		else if (data[0] == 2 && dataSize == sizeof(int))
		{
			// Disconnection Packet
			PacketController::disconnectClient(xid, false);
		}
		break;
	// ATTACK COLLISION
	case 2:
		if (dataSize == 3 * sizeof(int))
		{
			int hurtMonsterId = data[0];
			int projectileToBeDestroyed = data[1];
			int attack = data[2];

			//std::cout << "Projectile " << projectileToBeDestroyed << " hit a monster. It will be destroyed." << std::endl;

			Monster* hurtMonster = PacketController::game->getMonsterOfId(hurtMonsterId);
			if (hurtMonster != NULL)
				hurtMonster->doDamage(attack);

			//PacketController::game->destroyProjectileOfId(projectileToBeDestroyed);

			for (unsigned int i = 0; i < udpServer->getClients()->size(); i++)
				if ((*udpServer->getClients())[i]->id != xid || projectileToBeDestroyed == -1)
					udpServer->sendPackets(Packet(data, 3, 4, xid), (*udpServer->getClients())[i]->netInfo);
		}
		break;
	// MONSTER ATTACK COLLISION
	case 11:
		if (dataSize == 3 * sizeof(int))
		{
			int monsterId = data[0];
			int projectileToBeDestroyed = data[1];
			int attack = data[2];

			//std::cout << "Projectile " << projectileToBeDestroyed << " hit a monster. It will be destroyed." << std::endl;

			Player* hurtPlayer = PacketController::getPlayerOfClient(xid);
			if (hurtPlayer != NULL)
				hurtPlayer->doDamage(attack);

			//PacketController::game->destroyProjectileOfId(projectileToBeDestroyed);

			for (unsigned int i = 0; i < udpServer->getClients()->size(); i++)
				if ((*udpServer->getClients())[i]->id != xid || projectileToBeDestroyed == -1)
					udpServer->sendPackets(Packet(data, 3, 11, xid), (*udpServer->getClients())[i]->netInfo);
		}
		break;
	// PLAYER TOOK SIMPLE DAMAGE
	case 12:
		if (dataSize == sizeof(int))
		{
			Player* hurtPlayer = getPlayerOfClient(xid);
			int attack = data[0];
			if (hurtPlayer != NULL)
				hurtPlayer->doDamage(attack);

			// player hp will be updated in the next player attributes update.
		}
	}
}
void PacketController::dispatchFloatArray(int id, int xid, float* data, int dataSize)
{
	switch (id)
	{
	// SKILL EXECUTION
	case 6:
		Player* player = getPlayerOfClient(xid);

		if (player != NULL && (dataSize % (6 * sizeof(float)) == 0))
		{
			int numberOfSkills = dataSize / (6 * sizeof(float));

			for (int i = 0; i < numberOfSkills; i++)
			{
				SkillSlot skillSlot = SkillSlot((int)round(data[6 * i]));
				MovementDirection skillDirection = MovementDirection((int)round(data[6 * i + 1]));
				glm::vec3 skillTargetPosition = glm::vec3(data[6 * i + 2], data[6 * i + 3], data[6 * i + 4]);
				int creatureId = (int)data[6 * i + 5];

				Skill* usedSkill = player->getSkillOfSlot(skillSlot);

				if (usedSkill != NULL)
				{
					usedSkill->execute(skillDirection, skillTargetPosition, creatureId);

					for (unsigned int i = 0; i < udpServer->getClients()->size(); i++)
						udpServer->sendPackets(Packet(data, 6, 6, xid), (*udpServer->getClients())[i]->netInfo);
				}
			}
		}
		break;
	}
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
	Player* player;

	switch (id)
	{
	// PLAYER POSITION
	case 0:
		// REFRESH PLAYER POSITION
		player = getPlayerOfClient(xid);
		
		if (player != NULL && dataSize == sizeof(glm::vec3))
		{
			player->setPosition(*data);
		}

		break;

	// CREATE PROJECTILE (ATTACK)
	case 1:
		player = getPlayerOfClient(xid);

		if (player != NULL && dataSize == sizeof(glm::vec3))
		{
			glm::vec3 projectilePosition = player->getPosition();
			glm::vec3 projectileDirection = data[0];

			Projectile* newProjectile = PacketController::game->createNewProjectile(projectilePosition, projectileDirection, player->getId());

			std::cout << "Projectile " << newProjectile->getId() << " created." << std::endl;

			glm::vec4 info;
			info.x = data[0].x;
			info.y = data[0].y;
			info.z = data[0].z;
			info.w = (float)newProjectile->getId();

			for (unsigned int i = 0; i < udpServer->getClients()->size(); i++)
				udpServer->sendPackets(Packet(info, 3, xid), (*udpServer->getClients())[i]->netInfo);

			// projectile is being deleted because server side is not using it
			PacketController::game->destroyProjectileOfId(newProjectile->getId());
		}
		break;
	}
}
void PacketController::dispatchVec2fArray(int id, int xid, glm::vec2* data, int dataSize)
{

}
void PacketController::dispatchPing(int id, int xid)
{

}
void PacketController::dispatchPong(int id, int xid)
{
	for (unsigned int i = 0; i < udpServer->getClients()->size(); i++)
	{
		if ((*udpServer->getClients())[i]->id == xid)
		{
			(*udpServer->getClients())[i]->lastPong = LinkedTime::getTime();
			(*udpServer->getClients())[i]->setPing((*udpServer->getClients())[i]->lastPong - (*udpServer->getClients())[i]->lastPing);
		}
	}
}
void PacketController::disconnectClient(int clientId, bool timedOut)
{
	for (unsigned int i = 0; i < udpServer->getClients()->size(); i++)
	{
		if ((*udpServer->getClients())[i]->id == clientId)
		{
			if (timedOut)
			{
				std::cout << "Client id" << (*udpServer->getClients())[i]->id << " connection timed out" << std::endl;
			}
			else
			{
				char address[ADDRESS_IPV4_LENGHT] = { 0 };
				std::cout << "Client (" << inet_ntop((*udpServer->getClients())[i]->netInfo.sin_family, &(*udpServer->getClients())[i]->netInfo.sin_addr, address, ADDRESS_IPV4_LENGHT)
					<< ":" << (*udpServer->getClients())[i]->netInfo.sin_port << ") " <<
					"disconnected." << std::endl;
			}

			(*udpServer->getClients()).erase(udpServer->getClients()->begin() + i);

			// Delete client player
			for (unsigned int i = 0; i < PacketController::players->size(); i++)
				if ((*PacketController::players)[i]->getClientId() == clientId)
				{
					delete (*PacketController::players)[i];
					(*PacketController::players).erase((*PacketController::players).begin() + i);
				}

			for (unsigned int i = 0; i < udpServer->getClients()->size(); i++)
				udpServer->sendPackets(Packet(clientId, 7, 0), (*udpServer->getClients())[i]->netInfo);
		}
	}
}

void PacketController::sendEssentialGameInformationToClient(ClientInfo* client)
{
	Player* player = PacketController::game->createNewPlayer(udpServer->getClients()->back()->id);
	PacketController::createPlayerOnAllClientsExcept(client, player);
	PacketController::createAllPlayersOnClient(client);
	PacketController::createMonstersOnClient(client);
}

void PacketController::update10()
{
	PacketController::updatePlayersAttributesToAllClients();
	PacketController::updatePlayersPositionToAllClients();
	PacketController::updateMonstersPositionToAllClients();
	PacketController::updateMonstersAttacksToAllClients();
	PacketController::updateMonsterDamagesToAllClients();
}

void PacketController::queueMonsterAttack(int monsterId)
{
	monstersThatAttacked.insert(monsterId);
}

void PacketController::queueMonsterDamage(int monsterId, int projectileToBeDestroyed, int attack)
{
	monsterDamages.push_back(monsterId);
	monsterDamages.push_back(projectileToBeDestroyed);
	monsterDamages.push_back(attack);
}

void PacketController::createPlayerOnAllClientsExcept(ClientInfo* client, Player* newPlayer)
{
	short playerInformation[11];

	playerInformation[0] = newPlayer->getClientId();
	playerInformation[1] = newPlayer->getTotalMaximumHp();
	playerInformation[2] = newPlayer->getHp();
	playerInformation[3] = newPlayer->getTotalAttack();
	playerInformation[4] = newPlayer->getTotalDefense();
	playerInformation[5] = newPlayer->getTotalMagicalPower();
	playerInformation[6] = newPlayer->getTotalSpeed();
	playerInformation[7] = newPlayer->getTotalAttackSpeed();
	playerInformation[8] = (short)round(newPlayer->getPosition().x);
	playerInformation[9] = (short)round(newPlayer->getPosition().y);
	playerInformation[10] = (short)round(newPlayer->getPosition().z);

	for (unsigned int i = 0; i < udpServer->getClients()->size(); i++)
		if ((*udpServer->getClients())[i]->id != client->id)
			udpServer->sendPackets(Packet(playerInformation, 11, 0, 0), (*udpServer->getClients())[i]->netInfo);
}
void PacketController::createAllPlayersOnClient(ClientInfo* client)
{
	short playerInformation[11];

	for (Player* player : *PacketController::players)
	{
		playerInformation[0] = player->getClientId();
		playerInformation[1] = player->getTotalMaximumHp();
		playerInformation[2] = player->getHp();
		playerInformation[3] = player->getTotalAttack();
		playerInformation[4] = player->getTotalDefense();
		playerInformation[5] = player->getTotalMagicalPower();
		playerInformation[6] = player->getTotalSpeed();
		playerInformation[7] = player->getTotalAttackSpeed();
		playerInformation[8] = (short)round(player->getPosition().x);
		playerInformation[9] = (short)round(player->getPosition().y);
		playerInformation[10] = (short)round(player->getPosition().z);

		udpServer->sendPackets(Packet(playerInformation, 11, 0, 0), client->netInfo);
	}
}

void PacketController::createMonstersOnClient(ClientInfo* client)
{
	int monstersToCreate = PacketController::monsters->size();

	for (unsigned int i = 0; i < PacketController::monsters->size(); i += 8)
	{
		if (monstersToCreate >= 8)
		{
			PacketController::createMonsters(i, 8, client);
			monstersToCreate = monstersToCreate - 8;
		}
		else
		{
			PacketController::createMonsters(i, monstersToCreate, client);
		}
	}
}
void PacketController::createMonsters(int vectorIndex, int quantity, ClientInfo* client)
{
	short monstersInformation[64];

	for (int i = 0; i < quantity; i++)
	{
		Monster* monster = (*PacketController::monsters)[i + vectorIndex];

		monstersInformation[8 * i] = monster->getId();
		monstersInformation[8 * i + 1] = monster->getHp();
		monstersInformation[8 * i + 2] = monster->getMapColorRed();
		monstersInformation[8 * i + 3] = monster->getMapColorGreen();
		monstersInformation[8 * i + 4] = monster->getMapColorBlue();
		monstersInformation[8 * i + 5] = (short)round(monster->getPosition().x);
		monstersInformation[8 * i + 6] = (short)round(monster->getPosition().y);
		monstersInformation[8 * i + 7] = (short)(monster->getPosition().z);
	}

	udpServer->sendPackets(Packet(monstersInformation, 8*quantity, 0, 1), client->netInfo);
}

void PacketController::updatePlayersAttributesToAllClients()
{
	std::vector<Player*> playersThatShouldHaveTheirAttributesUpdated;

	for (Player* player : *PacketController::players)
	{
		if (player->didAttributesChanged())
		{
			playersThatShouldHaveTheirAttributesUpdated.push_back(player);
			player->resetAttributesChanged();
		}
	}

	int playersToUpdate = playersThatShouldHaveTheirAttributesUpdated.size();
#if SHOW_PACKETS_LOG
	if (playersToUpdate > 0)
		LOG("PACKET SENT TO CLIENTS: Player Attributes Changed");
#endif
	for (unsigned int i = 0; i < playersThatShouldHaveTheirAttributesUpdated.size(); i += 8)
	{
		if (playersToUpdate >= 8)
		{
			PacketController::updatePlayersAttributes(i, 8, playersThatShouldHaveTheirAttributesUpdated);
			playersToUpdate = playersToUpdate - 8;
		}
		else
		{
			PacketController::updatePlayersAttributes(i, playersToUpdate, playersThatShouldHaveTheirAttributesUpdated);
		}
	}
}
void PacketController::updatePlayersAttributes(int vectorIndex, int quantity, std::vector<Player*> players)
{
	short playerAttributes[64];

	for (int i = 0; i < quantity; i++)
	{
		Player* player = players[i + vectorIndex];

		playerAttributes[8 * i] = player->getClientId();
		playerAttributes[8 * i + 1] = player->getTotalMaximumHp();
		playerAttributes[8 * i + 2] = player->getHp();
		playerAttributes[8 * i + 3] = player->getTotalAttack();
		playerAttributes[8 * i + 4] = player->getTotalDefense();
		playerAttributes[8 * i + 5] = player->getTotalMagicalPower();
		playerAttributes[8 * i + 6] = player->getTotalSpeed();
		playerAttributes[8 * i + 7] = player->getTotalAttackSpeed();
	}

	for (unsigned int i = 0; i < udpServer->getClients()->size(); i++)
		udpServer->sendPackets(Packet(playerAttributes, 8 * quantity, 1, 0), (*udpServer->getClients())[i]->netInfo);
}

void PacketController::updatePlayersPositionToAllClients()
{
	// TO DO : remover player se tiver em fog of war
	std::vector<Player*> playersThatShouldHaveTheirPositionUpdated;

	for (Player* player : *PacketController::players)
		if (player->mustUpdateDestinationToClients())
			playersThatShouldHaveTheirPositionUpdated.push_back(player);

	int playersToUpdate = playersThatShouldHaveTheirPositionUpdated.size();

#if SHOW_PACKETS_LOG
	if (playersToUpdate > 0)
		LOG("PACKET SENT TO CLIENTS: Player Position Changed");
#endif

	for (unsigned int i = 0; i < playersThatShouldHaveTheirPositionUpdated.size(); i += 16)
	{
		if (playersToUpdate >= 16)
		{
			PacketController::updatePlayersPosition(i, 16, playersThatShouldHaveTheirPositionUpdated);
			playersToUpdate = playersToUpdate - 16;
		}
		else
		{
			PacketController::updatePlayersPosition(i, playersToUpdate, playersThatShouldHaveTheirPositionUpdated);
		}
	}
}
void PacketController::updatePlayersPosition(int vectorIndex, int quantity, std::vector<Player*> players)
{
	glm::vec3 playersPosition[16];
	short playersClientIds[16];

	for (int i = 0; i < quantity; i++)
	{
		Player* player = players[i + vectorIndex];

		playersPosition[i] = player->getDestination();
		playersClientIds[i] = player->getClientId();
	}

	for (unsigned int i = 0; i < udpServer->getClients()->size(); i++)
		udpServer->sendPackets(Packet(playersPosition, playersClientIds, quantity, 1, 1), (*udpServer->getClients())[i]->netInfo);
}

void PacketController::updateMonstersPositionToAllClients()
{
	for (Player* player : *PacketController::players)
	{
		std::vector<Monster*> monstersThatShouldHaveTheirPositionUpdated;

		for (Monster* monster : *PacketController::monsters)
			if (!player->isFogOfWar(monster->getPosition()) && monster->isAlive())
				monstersThatShouldHaveTheirPositionUpdated.push_back(monster);

		int monstersToUpdate = monstersThatShouldHaveTheirPositionUpdated.size();

#if SHOW_PACKETS_LOG
		if (monstersToUpdate > 0)
			LOG("PACKET SENT TO CLIENTS: Monster Position Changed");
#endif

		for (unsigned int i = 0; i < monstersThatShouldHaveTheirPositionUpdated.size(); i += 16)
		{
			if (monstersToUpdate >= 16)
			{
				PacketController::updateMonstersPosition(i, 16, monstersThatShouldHaveTheirPositionUpdated);
				monstersToUpdate = monstersToUpdate - 16;
			}
			else
			{
				PacketController::updateMonstersPosition(i, monstersToUpdate, monstersThatShouldHaveTheirPositionUpdated);
			}
		}
	}
}
void PacketController::updateMonstersPosition(int vectorIndex, int quantity, std::vector<Monster*> monsters)
{
	glm::vec3 monstersPosition[16];
	short monstersIds[16];

	for (int i = 0; i < quantity; i++)
	{
		Monster* monster = monsters[i + vectorIndex];
		monstersPosition[i] = monster->getPosition();
		monstersIds[i] = monster->getId();
	}

	for (unsigned int i = 0; i < udpServer->getClients()->size(); i++)
		udpServer->sendPackets(Packet(monstersPosition, monstersIds, quantity, 2, 0), (*udpServer->getClients())[i]->netInfo);
}

void PacketController::updateMonstersAttacksToAllClients()
{
	if (monstersThatAttacked.size() == 0)
		return;

	// transforms the set into a vector
	std::vector<int> monsterAttacks(monstersThatAttacked.begin(), monstersThatAttacked.end());

	int monstersAttacksToUpdate = monsterAttacks.size();

#if SHOW_PACKETS_LOG
	if (monstersAttacksToUpdate > 0)
		LOG("PACKET SENT TO CLIENTS: Monster Attacked");
#endif

	for (unsigned int i = 0; i < monsterAttacks.size(); i += 32)
	{
		if (monstersAttacksToUpdate >= 32)
		{
			PacketController::updateMonstersAttacks(i, 32, monsterAttacks);
			monstersAttacksToUpdate = monstersAttacksToUpdate - 32;
		}
		else
		{
			PacketController::updateMonstersAttacks(i, monstersAttacksToUpdate, monsterAttacks);
		}
	}

	monstersThatAttacked.clear();
}
void PacketController::updateMonstersAttacks(int vectorIndex, int quantity, std::vector<int> attacks)
{
	int monstersAttacks[32];

	for (int i = 0; i < quantity; i++)
	{
		int monsterId = attacks[i + vectorIndex];

		monstersAttacks[i] = monsterId;
	}

	for (unsigned int i = 0; i < udpServer->getClients()->size(); i++)
		udpServer->sendPackets(Packet(monstersAttacks, quantity, 5, 0), (*udpServer->getClients())[i]->netInfo);
}

void PacketController::updateMonsterDamagesToAllClients()
{
	if (monsterDamages.size() == 0)
		return;

	int monsterDamagesToUpdate = monsterDamages.size();

#if SHOW_PACKETS_LOG
	if (monsterDamagesToUpdate > 0)
		LOG("PACKET SENT TO CLIENTS: Monster Took Damage");
#endif

	for (unsigned int i = 0; i < monsterDamages.size(); i += 30)
	{
		if (monsterDamagesToUpdate >= 30)
		{
			PacketController::updateMonsterDamages(i, 30, monsterDamages);
			monsterDamagesToUpdate = monsterDamagesToUpdate - 30;
		}
		else
		{
			PacketController::updateMonsterDamages(i, monsterDamagesToUpdate, monsterDamages);
		}
	}

	monsterDamages.clear();
}
void PacketController::updateMonsterDamages(int vectorIndex, int quantity, std::vector<int> damages)
{
	int monsterDamages[30];

	for (int i = 0; i < quantity; i+=3)
	{
		int monsterId = damages[i + vectorIndex];
		int projectileToBeDestroyed = damages[i + vectorIndex + 1];
		int attack = damages[i + vectorIndex + 2];

		monsterDamages[i] = monsterId;
		monsterDamages[i + 1] = projectileToBeDestroyed;
		monsterDamages[i + 2] = attack;
	}

	for (unsigned int i = 0; i < udpServer->getClients()->size(); i++)
		udpServer->sendPackets(Packet(monsterDamages, quantity, 4, 0), (*udpServer->getClients())[i]->netInfo);
}

void PacketController::sendProjectileAttackByTurretMonster(int monsterId, Projectile* projectile)
{
	glm::vec4 info;
	info.x = projectile->getDirection().x;
	info.y = projectile->getDirection().y;
	info.z = projectile->getDirection().z;
	info.w = (float)projectile->getId();

	for (unsigned int i = 0; i < udpServer->getClients()->size(); i++)
		udpServer->sendPackets(Packet(info, 10, monsterId), (*udpServer->getClients())[i]->netInfo);
}

void PacketController::sendMonsterExplosionSkill(int monsterId)
{
	int dummy = -1;
	for (unsigned int i = 0; i < udpServer->getClients()->size(); i++)
		udpServer->sendPackets(Packet(dummy, 12, monsterId), (*udpServer->getClients())[i]->netInfo);
}

void PacketController::sendMonsterEndureSkill(int monsterId)
{
	int dummy = -1;
	for (unsigned int i = 0; i < udpServer->getClients()->size(); i++)
		udpServer->sendPackets(Packet(dummy, 13, monsterId), (*udpServer->getClients())[i]->netInfo);
}

void PacketController::dispatchMsg(int id, int xid, char* data, int senderID)
{
	std::string msg(data);
	std::cout << "Client (" << senderID << ") says to client " << id << ": " << msg << std::endl;
	if (id == -1)
	{
		// BroadCast
		for (unsigned int i = 0; i < udpServer->getClients()->size(); i++){
			if ((*udpServer->getClients())[i]->id != senderID)
				udpServer->sendPackets(Packet(msg, -1), (*udpServer->getClients())[i]->netInfo);
		}
	}
	else
	{
		// Personal message
		int receiverIndex = udpServer->clientExists(id);
		
		if (receiverIndex != -1 && id != senderID)
			udpServer->sendPackets(Packet(msg, senderID), (*udpServer->getClients())[receiverIndex]->netInfo);
	}
	
}

Player* PacketController::getPlayerOfClient(int clientId)
{
	for (Player* player : *(PacketController::players))
		if (player->getClientId() == clientId)
			return player;

	return NULL;
}