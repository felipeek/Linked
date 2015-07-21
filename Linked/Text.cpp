#include "Text.h"
#include "Primitive.h"
#include "Entity.h"
#include <glm\glm.hpp>

Text::Text(std::string text)
{
	this->text = text;
	this->length = text.length();

	fontTexture = new Texture("./res/Fonts/fontLinked3.png", -10);

	createQuads();
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

void Text::createQuads()
{
	float offset = -0.2f;
	float offsetz = 0;
	for (int i = 0; i < length; i++)
	{
		int c = decodeChar(this->text[i]);
		Quad* quad = new Quad(glm::vec3(0, 0, 0), QUADSIZE, QUADSIZE, ATLASROWS, c);
		Mesh* mesh = new Mesh(quad);
		if (c == _y)
			offsetz = 0.2f-0.023f;
		else
			offsetz = 0.2f;
		Transform* transform = new Transform(glm::vec3(offset, offsetz, 0), 0, glm::vec3(1,0,0), glm::vec3(0.1f, 0.1f, 0.1f));
		//transform->rotate(45, glm::vec3(1, 0, 0));
		offset += 0.055f;
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

std::vector<Entity*>& Text::getEntities()
{
	return chars;
}