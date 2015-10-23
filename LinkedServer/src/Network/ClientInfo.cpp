#include "ClientInfo.h"
#include "LinkedTime.h"

int ClientInfo::currentId = 0;

ClientInfo::ClientInfo(int id, struct sockaddr_in netInfo)
{
	this->id = id;
	this->netInfo = netInfo;
	this->lastPong = LinkedTime::getTime();
	this->lastPing = 0;
	ping = -1;
	ClientInfo::currentId++;
}


ClientInfo::~ClientInfo()
{
}

void ClientInfo::setPing(double val)
{
	this->ping = val * 1000.0;
}

double ClientInfo::getPing()
{
	return ping;
}