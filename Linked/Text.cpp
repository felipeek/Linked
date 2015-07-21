#include "Text.h"
#include "Primitive.h"
#include "Entity.h"
#include <glm\glm.hpp>

Text::Text(std::string text, float textSize)
{
	this->text = text;
	this->length = text.length();

	fontTexture = new Texture("./res/Fonts/fontLinked3.png", -10);

	createQuads(textSize);
}

Text::~Text()
{
	delete fontTexture;

	for (int i = 0; i < length; i++)
	{
		delete chars[i];
	}
	chars.clear();
}

void Text::createQuads(float textSize)
{
	float totalXoffset = 0.0f;
	float totalYoffset = 0.0f;

	float xoffset = 0.0f;

	for (int i = 0; i < length; i++)
	{
		int c = decodeChar(this->text[i]);
		Quad* quad = new Quad(glm::vec3(0, 0, 0), QUADSIZE, QUADSIZE, ATLASROWS, c);
		Mesh* mesh = new Mesh(quad);

		totalYoffset = getYoffset(c, textSize);
		xoffset = getXoffset(c, textSize);

		Transform* transform = new Transform(glm::vec3(totalXoffset + (xoffset / 2) - TEMPXOFFSET, totalYoffset, 0), glm::vec3(textSize * WIDTHFACTOR, textSize, textSize));

		totalXoffset += xoffset;

		Entity* entity = new Entity(transform, mesh, this->fontTexture);
		chars.push_back(entity);
	}
}

int Text::decodeChar(char c)
{
	switch (c)
	{
	case 'A': return _A; break;
	case 'B': return _B; break;
	case 'C': return _C; break;
	case 'D': return _D; break;
	case 'E': return _E; break;
	case 'F': return _F; break;
	case 'G': return _G; break;
	case 'H': return _H; break;
	case 'I': return _I; break;
	case 'J': return _J; break;
	case 'K': return _K; break;
	case 'L': return _L; break;
	case 'M': return _M; break;
	case 'N': return _N; break;
	case 'O': return _O; break;
	case 'P': return _P; break;
	case 'Q': return _Q; break;
	case 'R': return _R; break;
	case 'S': return _S; break;
	case 'T': return _T; break;
	case 'U': return _U; break;
	case 'V': return _V; break;
	case 'W': return _W; break;
	case 'X': return _X; break;
	case 'Y': return _Y; break;
	case 'Z': return _Z; break;

	case 'a': return _a; break;
	case 'b': return _b; break;
	case 'c': return _c; break;
	case 'd': return _d; break;
	case 'e': return _e; break;
	case 'f': return _f; break;
	case 'g': return _g; break;
	case 'h': return _h; break;
	case 'i': return _i; break;
	case 'j': return _j; break;
	case 'k': return _k; break;
	case 'l': return _l; break;
	case 'm': return _m; break;
	case 'n': return _n; break;
	case 'o': return _o; break;
	case 'p': return _p; break;
	case 'q': return _q; break;
	case 'r': return _r; break;
	case 's': return _s; break;
	case 't': return _t; break;
	case 'u': return _u; break;
	case 'v': return _v; break;
	case 'w': return _w; break;
	case 'x': return _x; break;
	case 'y': return _y; break;
	case 'z': return _z; break;

	case '0': return _0; break;
	case '1': return _1; break;
	case '2': return _2; break;
	case '3': return _3; break;
	case '4': return _4; break;
	case '5': return _5; break;
	case '6': return _6; break;
	case '7': return _7; break;
	case '8': return _8; break;
	case '9': return _9; break;

	case '.': return _DOT; break;
	case ':': return _COLON; break;
	case '!': return _EXCLAMATION; break;
	case '?': return _INTERROGATION; break;
	case '+': return _PLUS; break;
	case '-': return _MINUS; break;
	case '=': return _EQUAL; break;
	case '&': return _AMPERSAND; break;
	case ')': return _CLOSEPARENTHESIS; break;
	case '(': return _OPENPARENTHESIS; break;
	case '%': return _PERCENT; break;
	case ',': return _COMM4; break;
	case '/': return _SLASH; break;
	case '$': return _DOLLAR; break;
	case '*': return _STAR; break;
	case 27: return _APOSTROPHE; break;
	case ';': return _SEMICOLON; break;

	// Whitespace
	case ' ': return 0; break;

	default:
		return 0;
	}
}

float Text::getYoffset(int c, float textSize)
{
	switch (c)
	{
	case _A:
	case _B:
	case _C:
	case _D:
	case _E:
	case _F:
	case _G:
	case _H:
	case _I:
	case _J:
	case _K:
	case _L:
	case _M:
	case _N:
	case _O:
	case _P:
	case _Q:
	case _R:
	case _S:
	case _T:
	case _U:
	case _V:
	case _W:
	case _X:
	case _Y:
	case _Z: return 0.0f; break;

	case _a: return 0.0f; break;
	case _b: return 0.0f; break;
	case _c: return 0.0f; break;
	case _d: return 0.0f; break;
	case _e: return 0.0f; break;
	case _f: return 0.0f; break;
	case _g: return -textSize*alphaY; break;
	case _h: return 0.0f; break;
	case _i: return 0.0f; break;
	case _j: return -textSize*alphaY; break;
	case _k: return 0.0f; break;
	case _l: return 0.0f; break;
	case _m: return 0.0f; break;
	case _n: return 0.0f; break;
	case _o: return 0.0f; break;
	case _p: return -textSize*alphaY; break;
	case _q: return -textSize*alphaY; break;
	case _r: return 0.0f; break;
	case _s: return 0.0f; break;
	case _t: return 0.0f; break;
	case _u: return 0.0f; break;
	case _v: return 0.0f; break;
	case _w: return 0.0f; break;
	case _x: return 0.0f; break;
	case _y: return -textSize*alphaY; break;
	case _z: return 0.0f; break;

	case _0:
	case _1:
	case _2:
	case _3:
	case _4:
	case _5:
	case _6:
	case _7:
	case _8:
	case _9: return 0.0f; break;

	case _DOT:
	case _COLON:
	case _EXCLAMATION:
	case _INTERROGATION:
	case _PLUS:
	case _MINUS:
	case _EQUAL:
	case _AMPERSAND:
	case _CLOSEPARENTHESIS:
	case _OPENPARENTHESIS:
	case _PERCENT:
	case _COMM4:
	case _SLASH: 
	case _DOLLAR:
	case _STAR:
	case _APOSTROPHE:
	case _SEMICOLON: return 0.0f; break;

		// Whitespace
	case ' ': return 0.0f; break;

	default:
		return 0.0f;
	}
}

float Text::getXoffset(int c, float textSize)
{
	dividerX = 2.0f;
	switch (c)
	{
	case _A: return textSize * alphaX; break;
	case _B: return textSize * alphaX; break;
	case _C: return textSize * alphaX; break;
	case _D: return textSize * alphaX; break;
	case _E: return textSize * alphaX; break;
	case _F: return textSize * alphaX; break;
	case _G: dividerX = 0.8f; return (textSize / dividerX) * alphaX; break;
	case _H: return textSize * alphaX; break;
	case _I: return (textSize / dividerX) * alphaX; break;
	case _J: return (textSize / dividerX) * alphaX; break;
	case _K: return textSize * alphaX; break;
	case _L: return textSize * alphaX; break;
	case _M: dividerX = 0.7f; return (textSize / dividerX) * alphaX; break;
	case _N: return textSize * alphaX; break;
	case _O: dividerX = 0.8f; return (textSize / dividerX) * alphaX; break;
	case _P: return textSize * alphaX; break;
	case _Q: dividerX = 0.8f; return (textSize / dividerX) * alphaX; break;
	case _R: return textSize * alphaX; break;
	case _S: return textSize * alphaX; break;
	case _T: return textSize * alphaX; break;
	case _U: return textSize * alphaX; break;
	case _V: dividerX = 0.8f; return (textSize / dividerX) * alphaX; break;
	case _W: dividerX = 0.6f; return (textSize / dividerX) * alphaX; break;
	case _X: return textSize * alphaX; break;
	case _Y: return textSize * alphaX; break;
	case _Z: return textSize * alphaX; break;

	case _a: return textSize * alphaX; break;
	case _b: return textSize * alphaX; break;
	case _c: return textSize * alphaX; break;
	case _d: return textSize * alphaX; break;
	case _e: return textSize * alphaX; break;
	case _f: return (textSize / dividerX) * alphaX; break;
	case _g: return textSize * alphaX; break;
	case _h: return textSize * alphaX; break;
	case _i: return (textSize / dividerX) * alphaX; break;
	case _j: return (textSize / dividerX) * alphaX; break;
	case _k: return textSize * alphaX; break;
	case _l: return (textSize / dividerX) * alphaX; break;
	case _m: dividerX = 0.7f; return (textSize /dividerX) * alphaX; break;
	case _n: return textSize * alphaX; break;
	case _o: return textSize * alphaX; break;
	case _p: return textSize * alphaX; break;
	case _q: return textSize * alphaX; break;
	case _r: dividerX = 1.5f; return (textSize / dividerX) * alphaX; break;
	case _s: return textSize * alphaX; break;
	case _t: return (textSize / dividerX) * alphaX; break;
	case _u: return textSize * alphaX; break;
	case _v: return textSize * alphaX; break;
	case _w: dividerX = 0.7f; return (textSize / dividerX) * alphaX; break;
	case _x: return textSize * alphaX; break;
	case _y: return textSize * alphaX; break;
	case _z: return textSize * alphaX; break;

	case _0: return textSize * alphaX; break;
	case _1: dividerX = 1.8f; return (textSize / dividerX) * alphaX; break;
	case _2: return textSize * alphaX; break;
	case _3: return textSize * alphaX; break;
	case _4: return textSize * alphaX; break;
	case _5: return textSize * alphaX; break;
	case _6: return textSize * alphaX; break;
	case _7: return textSize * alphaX; break;
	case _8: return textSize * alphaX; break;
	case _9: return textSize * alphaX; break;


	case _DOT:				   return (textSize / dividerX) * alphaX; break;
	case _COLON:			   return (textSize / dividerX) * alphaX; break;
	case _EXCLAMATION:		   return (textSize / dividerX) * alphaX; break;
	case _INTERROGATION:	   return textSize * alphaX; break;
	case _PLUS:				   return textSize * alphaX; break;
	case _MINUS:			   return textSize * alphaX; break;
	case _EQUAL:			   return textSize * alphaX; break;
	case _AMPERSAND:		   return textSize * alphaX; break;
	case _CLOSEPARENTHESIS:	   return (textSize / dividerX) * alphaX; break;
	case _OPENPARENTHESIS:	   return (textSize / dividerX) * alphaX; break;
	case _PERCENT:			   return textSize * alphaX; break;
	case _COMM4:			   return (textSize / dividerX) * alphaX; break;
	case _SLASH:			   return textSize * alphaX; break;
	case _DOLLAR:			   return textSize * alphaX; break;
	case _STAR:				   return textSize * alphaX; break;
	case _APOSTROPHE:		   return textSize * alphaX; break;
	case _SEMICOLON:		   return (textSize / dividerX) * alphaX; break;

		// Whitespace
	case 0: dividerX = 1.5f; return (textSize / dividerX) * alphaX; break;

	default:
		return textSize * alphaX;
	}
}

std::vector<Entity*>& Text::getEntities()
{
	return chars;
}