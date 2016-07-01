#include "Configfiles.h"
#include <stdio.h>
#include "Logging/Log.h"

Config::Config(const char* filename, bool* mult, int* port, std::string& ip)
{
	ReadConfigFile(filename);
	ParseConfigFile();
	*mult = multiplayer;
	*port = serverport;
	ip = serverip;
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
		char* strs[] = { "multiplayer", "serverip", "serverport", "false", "true" };
		char buffer[3][256] = {};

		int i = 0;
		int j = 0;

		for (int x = 0; x < 3; x++)
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
		if (!strcmp(strs[3], buffer[0]))
			multiplayer = false;
		else if (!strcmp(strs[4], buffer[0]))
			multiplayer = true;
		else
			fatal("Config file incorrect, multiplayer requires boolean true/false.");

		serverip = std::string(buffer[1]);
		serverport = atoi(buffer[2]);
	}
}