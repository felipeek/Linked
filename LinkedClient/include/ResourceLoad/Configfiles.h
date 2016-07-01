#pragma once
#include <string>

class Config
{
public:
	Config(const char* filename, bool* mult, int* port, std::string& ip);
	~Config();
private:
	bool multiplayer;
	int serverport;
	std::string serverip;

	void ReadConfigFile(const char* filename);
	void ParseConfigFile();

	unsigned char* filedata;
	int filesize;
};