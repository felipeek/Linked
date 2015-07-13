#include "Display.h"
#include "Input.h"
#include "Time.h"
#include <Windows.h>

#include <GL\glew.h>
#include <GL\glut.h>
#include <iostream>
#include "Game.h"

Game* Display::game = NULL;
const double Display::frameTime = 1.0 / FRAMECAP;
double Display::unprocessedTime = 0;
double Display::frameCounter = 0;
double Display::passedTime = 0;
double Display::lastTime;
int Display::frames = 0;

Display::Display(int* argc, char** argv, std::string name)
{
	startGlut(argc, argv, name);
}

Display::~Display()
{
}

void Display::startGlut(int* argc, char** argv, std::string titulo)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WWID, WHEI);
	glutInitWindowPosition((HWID - WWID) / 2, (HHEI - WHEI) / 2);
	glutCreateWindow(titulo.c_str());

	glewExperimental = true;

	if (glewExperimental)
		std::cout << "Using glew experimental." << std::endl;

	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "Erro na chamada de glewInit()" << std::endl;
	}

	printf("OpenGL %s, GLSL %s\n",
		glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));


	glutDisplayFunc(MainLoop);												// Indica a função a ser utilizada em caso de mudança na janela
	glutIdleFunc(MainLoop);													// Função utilizada enquanto a janela não precisa responder eventos	
	//glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(KeyboardDownEvent);
	glutKeyboardUpFunc(KeyboardUpEvent);
	glutMouseFunc(MouseEvent);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassive);

	initOpenGL();
	glutMainLoop();
}

void Display::MainLoop()
{
	bool mustRender = false;
	double startTime = Time::getTime();
	passedTime = startTime - lastTime;
	lastTime = startTime;
	
	unprocessedTime += passedTime;
	frameCounter += passedTime;

	if (frameCounter >= 1.0)
	{
		//std::cout << frames << std::endl;
		frames = 0;
		frameCounter = 0;
	}
	while (unprocessedTime > frameTime)
	{
		mustRender = true;
		game->update();
		unprocessedTime -= frameTime;
	}

	if (mustRender)
	{
		render();
		frames++;
	}
	//else
		//Sleep(1);
}

void Display::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	game->render();
	glutSwapBuffers();
}

void Display::initOpenGL()
{
	glClearColor(CLEARCOLOR_R, CLEARCOLOR_G, CLEARCOLOR_B, CLEARCOLOR_A);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	game = new Game(WWID, WHEI);

	//Time
	lastTime = Time::getTime();
	frameCounter = 0;
	unprocessedTime = 0;
	frames = 0;
}

void Display::KeyboardDownEvent(unsigned char key, int x, int y)
{
	Input::keyStates[key] = true;
}
void Display::KeyboardUpEvent(unsigned char key, int x, int y)
{
	Input::keyStates[key] = false;
}
void Display::MouseEvent(int button, int state, int x, int y)
{

}
void Display::MouseMotion(int x, int y)
{

}
void Display::MousePassive(int x, int y)
{

}