#include "Input.h"

bool Input::keyStates[KEYSTATES] = { 0 };
int Input::wheel = 0;
bool Input::attack = false;
MouseAttack Input::mouseAttack;

void Input::clear()
{
	for (int i = 0; i < KEYSTATES; i++)
	{
		keyStates[i] = 0;
	}
	wheel = 0;
	attack = false;
}