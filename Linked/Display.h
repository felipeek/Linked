#pragma once
#include <string>
#include "Game.h"

#define WWID 1600
#define WHEI 900
#define HWID 1920
#define HHEI 1080
#define CLEARCOLOR_R 0.49f
#define CLEARCOLOR_G 0.75f
#define CLEARCOLOR_B 0.93f
#define CLEARCOLOR_A 1.0f
#define FRAMECAP 60.0

class Display
{
public:
	Display(int* argc, char** argv, std::string name);
	~Display();
private:
	static Game* game;
	static void startGlut(int* argc, char** argv, std::string titulo);
	
	static void MainLoop();
	static void KeyboardDownEvent(unsigned char key, int x, int y);
	static void KeyboardUpEvent(unsigned char key, int x, int y);
	static void MouseEvent(int button, int state, int x, int y);
	static void MouseMotion(int x, int y);
	static void MousePassive(int x, int y);

	static void initOpenGL();
	static void render();

	const static double frameTime;
	static double unprocessedTime;
	static double frameCounter;
	static double lastTime;
	static double startTime;
	static double passedTime;
	static int frames;
};

