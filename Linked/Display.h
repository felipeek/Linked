#pragma once
#include "../Game.h"

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Game;

#define WWID 800
#define WHEI 600
#define WINDOW_START_X 100
#define WINDOW_START_Y 100

#define CLEARCOLOR_R 0.49f
#define CLEARCOLOR_G 0.75f
#define CLEARCOLOR_B 0.93f
#define CLEARCOLOR_A 1.0f

#define FRAMECAP 60.0
#define GAMESPEED 60.0

class Display
{
public:
	Display(int* argc, char** argv, std::string name);
	~Display();
	const static double frameTime;
	static int monitorWidth;
	static int monitorHeight;
private:
	static Game* game;
	static GLFWwindow* window;
	static GLFWmonitor* monitor;
	static void startGlfw(int* argc, char** argv, std::string titulo);
	
	static void MainLoop(GLFWwindow* window);

	static void mousePosCallBack(GLFWwindow* window, double x, double y);
	static void wheelCallBack(GLFWwindow* window, double xoffset, double yoffset);
	static void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseCallBack(GLFWwindow* window, int button, int action, int mods);
	static void focusedCallBack(GLFWwindow* window, int focused);

	static void initOpenGL();
	static void getSystemInfo();
	static void render();
	
	static double totalTime;
	static double timeSinceLastUpdate;
	static double elapsedTime;
	static double sumTime;
	static double gameTime;
	static double frameCount;
	static double update10Time;
	static int frames;

	static void printOpenGLandGLSLversions();
};

