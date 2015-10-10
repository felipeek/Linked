#include "Display.h"
#include "Input.h"
#include "Time.h"
#include <iostream>
#include "Game.h"
#include "PacketController.h"
#include "Configuration.h"

// Window and Monitor
GLFWwindow* Display::window = NULL;
GLFWmonitor* Display::monitor = NULL;
int Display::monitorWidth = 0;
int Display::monitorHeight = 0;


// Game
Game* Display::game = NULL;

// Time
const double Display::frameTime = 1.0 / GAMESPEED;
double Display::totalTime = 0;
double Display::timeSinceLastUpdate = 0;
double Display::elapsedTime = 0;
double Display::sumTime = 0;
double Display::gameTime = 0;
double Display::frameCount = 0;
double Display::update10Time = 0;
int Display::frames = 0;

Display::Display(int* argc, char** argv, std::string name)
{
	startGlfw(argc, argv, name);
}

Display::~Display()
{
	if (game != NULL)
		delete game;
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Display::startGlfw(int* argc, char** argv, std::string titulo)
{
	if (glfwInit() == GL_FALSE)
	{
		std::cerr << "Error initializing glfw!" << std::endl;
		return;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WWID, WHEI, titulo.c_str(), NULL, NULL);

	if (!window)
	{
		std::cerr << "Error creating the window!" << std::endl;
		glfwTerminate();
		return;
	}
	
	// Get monitor size
	getSystemInfo();

	// Set window starting position
	glfwSetWindowPos(window, WINDOW_START_X, WINDOW_START_Y);

	glfwMakeContextCurrent(window);

	// Disable vsync
	glfwSwapInterval(0);

	// Set callbacks for input
	glfwSetKeyCallback(window, keyCallBack);
	glfwSetMouseButtonCallback(window, mouseCallBack);
	glfwSetScrollCallback(window, wheelCallBack);
	glfwSetCursorPosCallback(window, mousePosCallBack);
	glfwSetWindowFocusCallback(window, focusedCallBack);

	// TODO : check if needed
	glewExperimental = true;
	
	if (glewExperimental)
		std::cout << "Using glew experimental." << std::endl;

	// Start glew
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "Erro na chamada de glewInit()" << std::endl;
	}

	printOpenGLandGLSLversions();

	initOpenGL();

	MainLoop(window);
}

void Display::MainLoop(GLFWwindow* window)
{
	do{
		totalTime = glfwGetTime();

		if (timeSinceLastUpdate == 0)
			timeSinceLastUpdate = totalTime;

		elapsedTime = totalTime - timeSinceLastUpdate;

		timeSinceLastUpdate = totalTime;

		sumTime += elapsedTime;
		gameTime += elapsedTime;
		frameCount += elapsedTime;
		update10Time += elapsedTime;

		if (gameTime >= 1.0 / GAMESPEED)			// Updates GAMESPEED times per second
		{
			game->update();
			glfwPollEvents();
			gameTime = gameTime - (1.0/GAMESPEED);
		}
		if (sumTime >= 1.0 / FRAMECAP)				// Renders at most FRAMECAP times per second
		{
			sumTime = 0;
			render();
			frames++;
		}
#ifdef MULTIPLAYER
		if (update10Time >= 1.0 / 10)				// Send packets 10 times per second
		{
			update10Time = 0;
			PacketController::update10();
		}
#endif
		if (frameCount >= 1.0)
		{
			std::cout << frames << std::endl;
			frameCount = 0;
			frames = 0;
		}

	} while (glfwWindowShouldClose(window) == false);
}

void Display::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	game->render();
	glfwSwapBuffers(window);
}

void Display::initOpenGL()
{
	glClearColor(CLEARCOLOR_R, CLEARCOLOR_G, CLEARCOLOR_B, CLEARCOLOR_A);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	game = new Game(WWID, WHEI);
}

void Display::getSystemInfo()
{
	monitor = glfwGetPrimaryMonitor();
	GLFWvidmode* mode = (GLFWvidmode*)glfwGetVideoMode(monitor);
	monitorWidth = mode->width;
	monitorHeight = mode->height;
}

void Display::printOpenGLandGLSLversions()
{
	std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void Display::keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	bool value = false;
	if (action != 0)
		value = true;

	Input::keyStates[key] = value;
	if (key >= 'A' && key <= 'Z')
	{
		Input::keyStates[key + 32] = value;
	}
}

void Display::mouseCallBack(GLFWwindow* window, int button, int action, int mods)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	const float aspect = (float)width / height;
	float screenX = ((float)x / width - 0.5f) * aspect;
	float screenY = -((float)y / height - 0.5f) * aspect;
	
	if (button == 0)
	{
		//Input::attack = true;
		Input::attack = !Input::attack;
		Input::mouseAttack.setAttackPos(screenX, screenY);
		Input::mouseAttack.setMouseCoords((int)x, (int)y);
	}

	if (button == 1)
		Input::leftMouseButton = true;

	Input::mouseAttack.setAttackPos(screenX, screenY);
	Input::mouseAttack.setMouseCoords((int)x, (int)y);
}

void Display::mousePosCallBack(GLFWwindow* window, double x, double y)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	const float aspect = (float)width / height;
	float screenX = ((float)x / width - 0.5f) * aspect;
	float screenY = -((float)y / height - 0.5f);

	Input::mouseAttack.setAttackPos(screenX, screenY);
	Input::mouseAttack.setMouseCoords((int)x, (int)y);
}

void Display::wheelCallBack(GLFWwindow* window, double xoffset, double yoffset)
{
	Input::wheel = (int)yoffset;
}

void Display::focusedCallBack(GLFWwindow* window, int focused)
{
	if (focused != 1)
		Input::clear();

}