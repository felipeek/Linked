#include "Configfiles.h"
#include <stdio.h>
#include "Logging/Log.h"

Config::Config(const char* filename, bool* mult, int* port, std::string& ip, int* music_volume, int* effects_volume)
{
	ReadConfigFile(filename);
	ParseConfigFile();
	*mult = multiplayer;
	*port = serverport;
	ip = serverip;
	*music_volume = this->music_volume;
	*effects_volume = this->effects_volume;
}

Config::~Config()
{
	delete[] filedata;
}

void Config::ReadConfigFile(const char* filename)
{
	FILE* file = fopen(filename, "r");
	if (!file)
	{
		fatal("File ", filename, " not found.");
	}

	fseek(file, 0, SEEK_END);
	filesize = ftell(file);
	fseek(file, 0, SEEK_SET);

	filedata = new unsigned char[filesize + 1];
	filedata[filesize] = 0;
	int bytesread = fread(filedata, 1, filesize, file);
	fclose(file);
}

void Config::ParseConfigFile()
{
	if (filedata)
	{
		char* strs[] = { "multiplayer", "serverip", "serverport", "music_volume", "effects_volume" };
		char* bools[] = { "false", "true" };
		char buffer[5][256] = {};

		int i = 0;
		int j = 0;

		for (int x = 0; x < 5; x++)
		{
			while (filedata[i] == strs[x][j])
			{
				i++;
				j++;
			}

			if (filedata[i] == '=')
				i++;
			else
				fatal("Config file incorrect.");

			j = 0;
			while (filedata[i] != '\n')
			{
				buffer[x][j] = filedata[i];
				j++;
				i++;
				if (filedata[i] == '\0')
					break;
			}
			i++;
			j = 0;
		}
		if (!strcmp(bools[0], buffer[0]))
			multiplayer = false;
		else if (!strcmp(bools[1], buffer[0]))
			multiplayer = true;
		else
			fatal("Config file incorrect, multiplayer requires boolean true/false.");

		serverip = std::string(buffer[1]);
		serverport = atoi(buffer[2]);
		music_volume = atoi(buffer[3]);
		effects_volume = atoi(buffer[4]);
	}
}