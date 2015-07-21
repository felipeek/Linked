#pragma once
#include <string>
#include <sstream>
#include <vector>

class Entity;
class Texture;

#define QUADSIZE 0.5f
#define ATLASROWS 16
#define WIDTHFACTOR 0.8f
// temp
#define TEMPXOFFSET 0.96f
#define TEMPYOFFSET 0.75f

#define _DEFAULT 240

#define _A 240
#define _B 241
#define _C 242
#define _D 243
#define _E 244
#define _F 245
#define _G 246
#define _H 247
#define _I 248
#define _J 249
#define _K 250
#define _L 251
#define _M 252
#define _N 253
#define _O 254
#define _P 255
#define _Q 224
#define _R 225
#define _S 226
#define _T 227
#define _U 228
#define _V 229
#define _W 230
#define _X 231
#define _Y 232
#define _Z 233

#define _a 208
#define _b 209
#define _c 210
#define _d 211
#define _e 212
#define _f 213
#define _g 214
#define _h 215
#define _i 216
#define _j 217
#define _k 218
#define _l 219
#define _m 220
#define _n 221
#define _o 222
#define _p 223
#define _q 192
#define _r 193
#define _s 194
#define _t 195
#define _u 196
#define _v 197
#define _w 198
#define _x 199
#define _y 200
#define _z 201

#define _0 176
#define _1 177
#define _2 178
#define _3 179
#define _4 180
#define _5 181
#define _6 182
#define _7 183
#define _8 184
#define _9 185

#define _DOT 160
#define _COLON 161
#define _EXCLAMATION 162
#define _INTERROGATION 163
#define _PLUS 164
#define _MINUS 165
#define _EQUAL 166
#define _AMPERSAND 167
#define _CLOSEPARENTHESIS 168
#define _OPENPARENTHESIS 169
#define _PERCENT 170
#define _COMM4 171
#define _SLASH 172
#define _DOLLAR 173
#define _STAR 174
#define _APOSTROPHE 175
#define _SEMICOLON 144

class Text
{
public:
	Text(std::string text, float textSize);
	~Text();

	std::vector<Entity*>& getEntities();
private:
	Texture* fontTexture;
	std::vector<Entity*> chars;
	std::string text;
	int length;

	void createQuads(float textSize);
	int decodeChar(char c);
	float getYoffset(int c, float textSize);
	float getXoffset(int c, float textSize);

	const float alphaX = 0.55f;
	float dividerX;

	const float alphaY = 0.25f;
};

