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
#include <windows.h>
/*
// Display related
Display* Display::s_CurrentInstance;
const glm::vec4 Display::s_ClearColor = glm::vec4(0.49f, 0.75f, 0.93f, 1.0f);
glm::vec2 Display::s_CursorPosition;

// Window and Monitor

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

Display::Display(std::string& name, int width, int height)
	: m_width(width), m_height(height)
{
	s_CurrentInstance = this;
}

Display::~Display()
{
	if (game != nullptr)
		delete game;

	linked::Window::linkedWindowDestroy();
}

void Display::startGlfw(Display* display, int* argc, char** argv, std::string titulo)
{
	// Setup OpenGL context

	glewExperimental = true;
	// Start glew
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "Erro na chamada de glewInit()" << std::endl;
	}
#ifdef DEBUG
	PrintOpenGLVersion();
#endif
	initOpenGL();
}
*/
/*
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
*/
/*
void Display::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (game != nullptr)
		game->render();

	linked::Window::updateWindows();
	linked::Window::renderWindows();

	//glfwSwapBuffers(window);
}

void Display::initOpenGL()
{
	glClearColor(s_ClearColor.r, s_ClearColor.g, s_ClearColor.b, s_ClearColor.a);
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
	timeSinceLastUpdate = LinkedTime::getTime();		// Ignores time spent to instantiate game (fixed speed up when starting)
}

void Display::exitGame()
{
	shouldExit = true;
}

void Display::PrintOpenGLVersion()
{
	std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

bool Display::isWindowFocused()
{
	return (int)GetFocus() == windowHandle;
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
}*/