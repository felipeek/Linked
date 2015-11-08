#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

class Game;

//#define WWID 1600
//#define WHEI 900
#define WINDOW_START_X 100
#define WINDOW_START_Y 100

#define CLEARCOLOR glm::vec4(0.49f, 0.75f, 0.93f, 1.0f)

#define FRAMECAP 60.0
#define GAMESPEED 60.0

class Display
{
public:
	Display(int* argc, char** argv, std::string name, int width, int height);
	~Display();
	const static double frameTime;
	static int monitorWidth;
	static int monitorHeight;
	static const Display& getCurrentInstance();
private:
	int m_width, m_height;

	static Display* currentInstance;
	static glm::vec4 clearColor;

	static Game* game;
	static GLFWwindow* window;
	static GLFWmonitor* monitor;
	static void startGlfw(Display* display, int* argc, char** argv, std::string titulo);
	
	static void MainLoop(GLFWwindow* window);

	static void mousePosCallBack(GLFWwindow* window, double x, double y);
	static void wheelCallBack(GLFWwindow* window, double xoffset, double yoffset);
	static void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseCallBack(GLFWwindow* window, int button, int action, int mods);
	static void focusedCallBack(GLFWwindow* window, int focused);
	static void resizeCallback(GLFWwindow* window, int width, int height);

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
public:
	int getWidth()const{ return m_width; }
	int getHeight()const{ return m_height; }
};

