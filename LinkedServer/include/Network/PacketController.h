#pragma once

#include "glm\glm.hpp"
#include <vector>
#include <set>
#include <winsock2.h>

class ClientPacket;
class Player;
class UDPServer;
class ClientInfo;
class Monster;
class Game;
class Projectile;

class PacketController
{
public:
	static Player* getPlayerOfClient(int clientId);

	static void dispatch(ClientPacket* cp);

	static UDPServer* udpServer;

	static std::vector<Player*>* players;
	static std::vector<Monster*>* monsters;
	static Game* game;

	// Function below must be optimized.
	static void sendProjectileAttackByTurretMonster(int monsterId, Projectile* direction);

	static void disconnectClient(int clientId, bool timedOut);
	static void update10();

	static bool isConnectionPacket(ClientPacket* cp);

	static void queueMonsterAttack(int monsterId);
	static void queueMonsterDamage(int monsterId, int projectileToBeDestroyed, int attack);

private:
	static void dispatchByteArray(int id, int xid, char* data, int dataSize);
	static void dispatchShortArray(int id, int xid, short* data, int dataSize);
	static void dispatchIntArray(int id, int xid, int* data, int dataSize);
	static void dispatchFloatArray(int id, int xid, float* data, int dataSize);
	static void dispatchLongArray(int id, int xid, long* data, int dataSize);
	static void dispatchDoubleArray(int id, int xid, double* data, int dataSize);
	static void dispatchVec4fArray(int id, int xid, glm::vec4* data, int dataSize);
	static void dispatchVec3fArray(int id, int xid, glm::vec3* data, int dataSize);
	static void dispatchVec2fArray(int id, int xid, glm::vec2* data, int dataSize);
	static void dispatchPing(int id, int xid);
	static void dispatchPong(int id, int xid);
	static void dispatchMsg(int id, int xid, char* data, int senderID);

	static void sendEssentialGameInformationToClient(ClientInfo* client);

	static void createPlayerOnAllClientsExcept(ClientInfo* client, Player* newPlayer);
	static void createAllPlayersOnClient(ClientInfo* client);

	static void createMonstersOnClient(ClientInfo* client);
	static void createMonsters(int vectorIndex, int quantity, ClientInfo* client);

	static void updatePlayersAttributesToAllClients();
	static void updatePlayersAttributes(int vectorIndex, int quantity, std::vector<Player*> players);

	static void updatePlayersPositionToAllClients();
	static void updatePlayersPosition(int vectorIndex, int quantity, std::vector<Player*> players);

	static void updateMonstersPositionToAllClients();
	static void updateMonstersPosition(int vectorIndex, int quantity, std::vector<Monster*> monsters);

	static void updateMonstersAttacksToAllClients();
	static void updateMonstersAttacks(int vectorIndex, int quantity, std::vector<int> attacks);

	static void updateMonsterDamagesToAllClients();
	static void updateMonsterDamages(int vectorIndex, int quantity, std::vector<int> damages);

	static std::set<int> monstersThatAttacked;
	static std::vector<int> monsterDamages;

	static ClientPacket* clientPacket;
};