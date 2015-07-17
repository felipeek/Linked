#pragma once
#include <vector>
#include <string>

#define FILE_EXTENSION ".xml"
#define READ_DIRECTORY_ERROR "Error trying to read directory"

class Factory
{
public:
	Factory();
	~Factory();
protected:
	std::vector<std::string> getListOfFilesInDirectory(char* directory);
};

