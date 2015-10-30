#pragma once
#include "Entity.h"

#define CURSOR_ROTATION_SPEED 1.4f

class Cursor : public Entity
{
public:
	Cursor();
	~Cursor();
	void update();
private:
	float cursorRot;
};

