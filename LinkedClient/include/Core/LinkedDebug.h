#pragma once
#include "Player.h"
#include <iostream>
#include <string>
#include <sstream>

class LinkedDebug
{
public:
	LinkedDebug()
	{
	}

	std::string PrintAtAddress(void* address, int offset)
	{
		std::stringstream ss;
		ss << *(int*)((char*)address + offset);
		return ss.str();
	}

	void SetIntegerValueAtAddress(void* address, int offset, int value)
	{
		*(int*)((char*)address + offset) = value;
	}
};