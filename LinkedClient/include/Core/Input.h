#pragma once
#include "MouseAttack.h"

#define KEYSTATES 512
#include <glm/glm.hpp>

class Input
{
public:
	static bool keyStates[KEYSTATES];
	static bool attack;
	static bool leftMouseButton;
	static MouseAttack mouseAttack;
	static int wheel;
	static bool mouseDown;
	static glm::vec2 mousePos;
	inline static glm::vec2& GetMousePos() { return mousePos; }

	static void clear();
};

// Special Keys
#define KEY_APOSTROPHE 96
#define KEY_COMMA 44
#define KEY_MINUS 45
#define KEY_PERIOD 46
#define KEY_SEMICOLON 47
#define KEY_EQUAL 61
#define KEY_LEFTBRACKET 93
#define KEY_RIGHTBRACKET 92
#define KEY_BACKSLASH 162
#define KEY_TAB 258
#define KEY_FSLASH 331
#define KEY_CCEDILHA 59	// BR keyboard
#define KEY_TILDA 39
#define KEY_ACUTE_ACCENT 91 // breaks TextRenderer

// numpad
#define KEY_STAR_NUM 332
#define KEY_MINUS_NUM 333
#define KEY_PLUS_NUM 334
#define KEY_COMMA_NUM 330

// Numeric Keys
#define KEY_0 48
#define KEY_1 49
#define KEY_2 50
#define KEY_3 51
#define KEY_4 52
#define KEY_5 53
#define KEY_6 54
#define KEY_7 55
#define KEY_8 56
#define KEY_9 57

/* -1 KEYS */
#define FSLASH_SCANCODE 115
#define KEY_PERIOD_NUM_SCANCODE 126