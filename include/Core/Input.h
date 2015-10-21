#pragma once
#include "MouseAttack.h"

#define KEYSTATES 512

class Input
{
public:
	static bool keyStates[KEYSTATES];
	static bool attack;
	static bool leftMouseButton;
	static MouseAttack mouseAttack;
	static int wheel;

	static void clear();
};

