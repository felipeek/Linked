#pragma once
#include <vector>
#include <string>

#define FACTORY_FILE_EXTENSION ".xml"
#define FACTORY_READ_DIRECTORY_ERROR "Error trying to read directory"

class Factory
{
public:
	Factory();
	~Factory();
protected:
	std::vector<std::string> getListOfFilesInDirectory(char* directory);
};

