#include "Factory.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>


Factory::Factory()
{
}


Factory::~Factory()
{
}

std::vector<std::string> Factory::getListOfFilesInDirectory(char* directory)
{
	DIR *dir;
	struct dirent *ent;
	char* aux;
	std::vector<std::string> fileNames;

	if ((dir = opendir(directory)) != NULL)
	{
		while ((ent = readdir(dir)) != NULL) {
			aux = ent->d_name;
			if (strstr(aux, FACTORY_FILE_EXTENSION))
				fileNames.push_back(std::string(aux));
		}

		closedir(dir);
	}
	else
	{
		perror(FACTORY_READ_DIRECTORY_ERROR);
	}

	return fileNames;
}