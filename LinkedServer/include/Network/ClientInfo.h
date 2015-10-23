#pragma once

#include <winsock2.h>

class ClientInfo
{
public:
	ClientInfo(int id, struct sockaddr_in netInfo);
	~ClientInfo();

	static int currentId;

	int id;
	struct sockaddr_in netInfo;

	double lastPong;
	double lastPing;

	void setPing(double val);
	double getPing();
private:
	double ping;
};

