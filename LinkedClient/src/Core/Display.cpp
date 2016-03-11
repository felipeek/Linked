#include "Display.h"
#include "Input.h"
#include "LinkedTime.h"
#include "Game.h"
#include "PacketController.h"
#include "Chat.h"
#include "Window.h"
#include "Common.h"
#include "GUI.h"
#include "Menu.h"

// Display related
Display* Display::currentInstance;
glm::vec4 Display::clearColor = CLEARCOLOR;
glm::vec2 Display::cursorPosition;

// Window and Monitor
GLFWwindow* Display::window = nullptr;
GLFWmonitor* Display::monitor = nullptr;
int Display::monitorWidth = 0;
int Display::monitorHeight = 0;

// Game
Game* Display::game = nullptr;
bool Display::shouldExit = false;
bool Display::shouldGoToMenu = false;
bool Display::gameIsInitialized = false;

// Time
double Display::totalTime = 0;
double Display::timeSinceLastUpdate = 0;
double Display::elapsedTime = 0;
double Display::sumTime = 0;
double Display::gameTime = 0;
double Display::frameCount = 0;
double Display::update10Time = 0;
int Display::frames = 0;

Display::Display(int* argc, char** argv, std::string name, int width, int height)
	: m_width(width), m_height(height)
{
	currentInstance = this;
	startGlfw(this, argc, argv, name);
}

Display::~Display()
{
	if (game != nullptr)
		delete game;

	linked::Window::linkedWindowDestroy();
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Display::startGlfw(Display* display, int* argc, char** argv, std::string titulo)
{
	if (glfwInit() == GL_FALSE)
	{
		std::cerr << "Error initializing glfw!" << std::endl;
		return;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(Display::getCurrentInstance().getWidth(), Display::getCurrentInstance().getHeight(), titulo.c_str(), NULL, NULL);

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
	//glfwSwapInterval(0);

	// Set callbacks for input
	glfwSetKeyCallback(window, keyCallBack);
	glfwSetMouseButtonCallback(window, mouseCallBack);
	glfwSetScrollCallback(window, wheelCallBack);
	glfwSetCursorPosCallback(window, mousePosCallBack);
	glfwSetWindowFocusCallback(window, focusedCallBack);
	glfwSetWindowSizeCallback(window, resizeCallback);

	// TODO : check if needed
	glewExperimental = true;
	
#ifdef DEBUG
	if (glewExperimental)
		std::cout << "Using glew experimental." << std::endl;
#endif

	// Start glew
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "Erro na chamada de glewInit()" << std::endl;
	}
#ifdef DEBUG
	printOpenGLandGLSLversions();
#endif
	initOpenGL();

	MainLoop(window);
}

void Display::MainLoop(GLFWwindow* window)
{
	do{
		totalTime = LinkedTime::getTime();

		if (timeSinceLastUpdate == 0)
			timeSinceLastUpdate = totalTime;

		elapsedTime = totalTime - timeSinceLastUpdate;

		timeSinceLastUpdate = totalTime;

		sumTime += elapsedTime;
		gameTime += elapsedTime;
		frameCount += elapsedTime;
		update10Time += elapsedTime;

		if (gameTime >= 1.0 / GAMESPEED)				// Updates GAMESPEED times per second
		{
			if (gameIsInitialized)
			{
				game->update();
				game->input();
			}
			Menu::update();
			glfwPollEvents();
			gameTime = gameTime - (1.0 / GAMESPEED);
		}
		if (sumTime >= 1.0 / FRAMECAP)					// Renders at most FRAMECAP times per second
		{
			sumTime = 0;
			render();
			frames++;
		}
		if (Game::multiplayer && game != nullptr)
		{
			if (update10Time >= 1.0 / 7)				// Send packets 7 times per second
			{
				update10Time = 0;
				PacketController::update10();
			}
		}

		if (frameCount >= 1.0)
		{
#ifdef DEBUG
			//std::cout << frames << std::endl;
#endif
			frameCount = 0;
			frames = 0;
		}
	} while (glfwWindowShouldClose(window) == false && !shouldExit);
}

void Display::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (game != nullptr)
		game->render();

	linked::Window::updateWindows();
	linked::Window::renderWindows();

	glfwSwapBuffers(window);
}

void Display::initOpenGL()
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	linked::Window::linkedWindowInit();
	Menu::setStateMenu(MENU_ACTIVE);
	Menu::update();

	game = nullptr;
}

void Display::startGame()
{
	if (!gameIsInitialized)
	{
		gameIsInitialized = true;
		game = new Game(Display::getCurrentInstance().getWidth(), Display::getCurrentInstance().getHeight());
	}
	// Hide cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	timeSinceLastUpdate = LinkedTime::getTime();		// Ignores time spent to instantiate game (fixed speed up when starting)
}

void Display::exitGame()
{
	shouldExit = true;
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
		Input::keyStates[key + 32] = value;

	Chat::update(key, scancode, action, mods);
}

void Display::mouseCallBack(GLFWwindow* window, int button, int action, int mods)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	const float aspect = (float)width / height;
	float screenX = ((float)x * 2 / width - 1.0f);
	float screenY = -((float)y * 2 / height - 1.0f);
	
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

	for (unsigned int i = 0; i < linked::Window::openedWindows.size(); i++)
		linked::Window::openedWindows[i]->mouseCallback(button, action, mods);
	linked::Button::mouseCallback(button, action, mods);
}

void Display::mousePosCallBack(GLFWwindow* window, double x, double y)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	const float aspect = (float)width / height;
	float screenX = ((float)x * 2 / width - 1.0f);
	float screenY = -((float)y * 2 / height - 1.0f);

	Input::mouseAttack.setAttackPos(screenX, screenY);
	Input::mouseAttack.setMouseCoords((int)x, (int)y);

	Display::currentInstance->cursorPosition.x = (float)x;
	Display::currentInstance->cursorPosition.y = (float)y;
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

void Display::resizeCallback(GLFWwindow* window, int width, int height)
{
	// Set the new size keeping the aspect ratio
	int newHeight = (int)(width * (1.0f / D_ASPECTRATIO));
	glfwSetWindowSize(window, width, newHeight);

	glViewport(0, 0, width, newHeight);

	Display::currentInstance->m_width = width;
	Display::currentInstance->m_height = newHeight;

	if (game)
	{
		game->setWindowWidth(width);
		game->setWindowHeight(newHeight);

		game->getGui()->resizeCallback(width, newHeight);
	}
}

const Display& Display::getCurrentInstance()
{
	return *Display::currentInstance;
}

void Display::destroyGameAndGotoMenu()
{
	shouldGoToMenu = true;
}