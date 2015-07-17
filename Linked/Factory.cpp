#include "Factory.h"
#include <dirent.h>

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
			if (strstr(aux, FILE_EXTENSION))
				fileNames.push_back(std::string(aux));
		}

		closedir(dir);
	}
	else
	{
		perror(READ_DIRECTORY_ERROR);
	}

	return fileNames;
}