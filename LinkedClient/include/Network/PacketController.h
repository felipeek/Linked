#pragma once

#include <glm\glm.hpp>
#include <vector>
#include "Movement.h"

class ClientPacket;
class Player;
class UDPClient;
class Map;
class Game;
class GUI;
enum SkillSlot;

// Entity's IDs
// Second Player 0

class PacketController
{
public:
	static Player* getPlayerOfClient(int clientId);

	/* REFERENCES */
	static UDPClient* udpClient;
	static Player* localPlayer;
	static std::vector<Player*>* onlinePlayers;
	static Game* game;
	static GUI* gui;

	/* METHODS */
	static void dispatch(ClientPacket* cp);
	static void update10();
	static void sendAttackToServer(glm::vec3 attackDirection);
	static void sendAttackCollisionToServer(int monsterId, int attackId, int damage);
	static void sendMonsterAttackCollisionToServer(int monsterId, int attackId, int damage);
	static void sendPlayerDamageToServer(int damage);

	/* SKILLS */
	static void sendSkillToServer(SkillSlot slot, MovementDirection skillDirection, glm::vec3 skillTargetPosition, int targetCreatureId);
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
	static void dispatchVec3fWithShortArray(int id, int xid, glm::vec3* data, short* extraData, int dataSize);
	static void dispatchMsg(int id, int xid, char* data);

	/* AUXILIAR */
	static glm::vec3 playerLastPosition;
};

