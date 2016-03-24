#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>

class Game;

class Display
{
public:
	Display(std::string& name, int width, int height);
	~Display();
	
	static const Display& getCurrentInstance();
	static void startGame();
	static void exitGame();
	static void destroyGameAndGotoMenu();
	static bool isWindowFocused();
	static int windowHandle;
private:
	int m_width, m_height;
	static bool shouldExit;
	static bool shouldGoToMenu;
	static bool gameIsInitialized;

	static const glm::vec4 s_ClearColor;
	static Display* s_CurrentInstance;
	static glm::vec2 s_CursorPosition;
	
	static int monitorWidth;
	static int monitorHeight;

	static Game* game;
	static void startGlfw(Display* display, int* argc, char** argv, std::string titulo);
	
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

	static void PrintOpenGLVersion();
public:
	int getWidth()const{ return m_width; }
	int getHeight()const{ return m_height; }
	static const glm::vec4& getClearColor() { return s_ClearColor; }
	static const glm::vec2& getCursorPosition(){ return s_CursorPosition; }
};

