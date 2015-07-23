#pragma once
#include "MouseAttack.h"

#define KEYSTATES 256

class Input
{
public:
	static bool keyStates[KEYSTATES];
	static bool attack;
	static MouseAttack mouseAttack;
	static int wheel;

	static void clear();

};

