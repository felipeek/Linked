#pragma once
#include <string>

class Config
{
public:
	Config(const char* filename, bool* mult, int* port, std::string& ip, int* music_volume, int* effects_volume);
	~Config();
private:
	bool multiplayer;
	int serverport;
	int music_volume, effects_volume;
	std::string serverip;

	void ReadConfigFile(const char* filename);
	void ParseConfigFile();

	unsigned char* filedata;
	int filesize;
};