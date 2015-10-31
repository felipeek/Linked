#pragma once
#include "Entity.h"

#define CURSOR_ROTATION_SPEED 1.4f

enum class CursorMode
{
	ATTACK,
	NORMAL
};

class Cursor : public Entity
{
public:
	Cursor();
	~Cursor();
	void update();
	void setCursorMode(CursorMode mode);
	void renderCursor(Shader* shader);
	void hideCursor();
	void showCursor();
private:
	Texture* normalCursorTexture;
	Texture* attackCursorTexture;
	float cursorRot;
	CursorMode mode;
	bool hide;
};

