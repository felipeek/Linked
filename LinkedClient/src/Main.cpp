#include <windows.h>
#include <time.h>

#include "Common.h"
#include "Display.h"
#include "native/ContextWindow.h"
#include "Core/Game.h"
#include "Core/LinkedTime.h"

#if _MSC_VER < 1900
// VC 2013
#pragma comment(lib, "./lib/vc2013/glew32s_vc2013.lib")
#pragma comment(lib, "./lib/vc2013/freetype261_vc2013.lib")
#pragma comment(lib, "./lib/vc2013/glfw3.lib")
#if _DEBUG
#pragma comment(lib, "./lib/vc2013/sfml-audio-d.lib")
#else
#pragma comment(lib, "./lib/vc2013/sfml-audio.lib")
#endif
#elif _MSC_VER < 2000
// VC 2015
#pragma comment(lib, "./lib/vc2015/glew32s_vc2015.lib")
#pragma comment(lib, "./lib/vc2015/freetype261_vc2015.lib")
#pragma comment(lib, "./lib/vc2015/glfw3.lib")
#if _DEBUG
#pragma comment(lib, "./lib/vc2015/sfml-audio-d.lib")
#else
#pragma comment(lib, "./lib/vc2015/sfml-audio.lib")
#endif
#endif
#pragma comment(lib, "opengl32.lib")
/*
int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));
	Display* display = new Display("Linked", 1600/2, 900/2);
	delete display;
	return 0;
}*/

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
	window = new ContextWindow(nCmdShow, hInstance, 800 * 1.5f, 450 * 1.5f, std::string("hoengine_opengl"), std::string("HoEngine"));
	window->InitOpenGL();
	game = new Game();


#if DEBUG
	AllocConsole();
	FILE* pCout;
	freopen_s(&pCout, "CONOUT$", "w", stdout);
#endif

	// Message/Game Loop
	const double FPS = 120.0;
	game->running = true;

	while (game->running)
	{
		static double StartTime = LinkedTime::getTime();
		static double EndTime = 0.0;
		static double Elapsed = 0.0;
		static double GameTime = 0.0;
		static double SecondCount = 0.0;
		static int Frames = 0;


		EndTime = LinkedTime::getTime();
		Elapsed = EndTime - StartTime;
		GameTime += Elapsed;
		SecondCount += Elapsed;

		if (SecondCount >= 1.0)
		{
			std::cout << Frames << std::endl;
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

		if (GameTime >= 1.0 / FPS)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			game->update();
			game->render();
			SwapBuffers(window->getHDC());
			GameTime = GameTime - (1.0 / FPS);
			Frames++;
		}
		else
			Sleep(1);

	}

	delete game;
	delete window;

#if DEBUG
	//fclose(pCout);	// release console
#endif
	return (int)msg.wParam;
}

