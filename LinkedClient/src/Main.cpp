#include <windows.h>
#include <time.h>

#include "Common.h"
#include "GL/glew.h"
#include "native/ContextWindow.h"
#include "Core/Game.h"
#include "Core/LinkedTime.h"
#include "Window.h"
#include "Input.h"
#include "PacketController.h"

#if _MSC_VER < 1900
// VC 2013
#pragma comment(lib, "./lib/vc2013/glew32s_vc2013.lib")
#pragma comment(lib, "./lib/vc2013/freetype261_vc2013.lib")
#if _DEBUG
#pragma comment(lib, "./lib/vc2013/sfml-audio-d.lib")
#else
#pragma comment(lib, "./lib/vc2013/sfml-audio.lib")
#endif
#elif _MSC_VER < 2000
// VC 2015
#pragma comment(lib, "./lib/vc2015/glew32s_vc2015.lib")
#pragma comment(lib, "./lib/vc2015/freetype261_vc2015.lib")
#if _DEBUG
#pragma comment(lib, "./lib/vc2015/sfml-audio-d.lib")
#else
#pragma comment(lib, "./lib/vc2015/sfml-audio.lib")
#endif
#endif
#pragma comment(lib, "opengl32.lib")

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	ContextWindow* window;
	Game* game;

	MSG msg;
	bool running = true;

	// Initialize window, OpenGL context and game
	window = new ContextWindow(nCmdShow, hInstance, 800 * 1.5, 450 * 1.5, std::string("hoengine_opengl"), std::string("Linked - v2.0"));
	window->InitOpenGL();
	game = new Game(window->getWidth(), window->getHeight());

#if DEBUG
	AllocConsole();
	FILE* pCout;
	freopen_s(&pCout, "CONOUT$", "w", stdout);
#endif
	//ShowCursor(false);

	glewExperimental = true;
	// Start glew
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "Erro na chamada de glewInit()" << std::endl;
	}

	// Message/Game Loop
	const double FPS = 120.0;
	const double GAMESPEED = 60.0;
	game->running = true;

	while (game->running)
	{
		static double StartTime = LinkedTime::getTime();
		static double EndTime = 0.0;
		static double Elapsed = 0.0;
		static double GameTime = 0.0;
		static double RenderTime = 0.0;
		static double SecondCount = 0.0;
		static double Update10Time = 0.0;
		static int Frames = 0;


		EndTime = LinkedTime::getTime();
		Elapsed = EndTime - StartTime;
		GameTime += Elapsed;
		RenderTime += Elapsed;
		SecondCount += Elapsed;
		Update10Time += Elapsed;

		if (SecondCount >= 1.0)
		{
#ifdef _DEBUG
			LOG(Frames);
#endif
			Frames = 0;
			SecondCount = 0;
		}

		StartTime = LinkedTime::getTime();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				game->running = false;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (GameTime >= 1.0 / GAMESPEED)
		{
			game->update();
			game->input();
			linked::Window::updateWindows();
			
			GameTime = GameTime - (1.0 / GAMESPEED);
		}
		else
			Sleep(1);

		if (Game::multiplayer && game != nullptr)
		{
			if (Update10Time >= 1.0 / 7)				// Send packets 7 times per second
			{
				Update10Time = 0;
				PacketController::update10();
			}
		}

		if (RenderTime >= 1.0 / FPS)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Render GUI and game
			game->render();
			linked::Window::renderWindows();

			SwapBuffers(window->getHDC());

			// Time control
			RenderTime = RenderTime - (1.0 / GAMESPEED);
			Frames++;
		}

	}

	delete game;
	delete window;

#if DEBUG
	fclose(pCout);	// release console
#endif
	return (int)msg.wParam;
}

