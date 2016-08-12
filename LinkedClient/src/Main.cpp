#include "Common.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	ContextWindow* window;
	Game* game;

	MSG msg;
	bool running = true;

#if _DEBUG
	AllocConsole();
	FILE* pCout;
	freopen_s(&pCout, "CONOUT$", "w", stdout);
#endif

	// Initialize window, OpenGL context and game
	window = new ContextWindow(nCmdShow, hInstance, 1440, 910, std::string("hoengine_opengl"), std::string("Linked - v2.0"));
	window->InitOpenGL();

	game = new Game(window->getWidth(), window->getHeight());
	//ShowCursor(false);

	glewExperimental = true;
	// Start glew
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "Erro na chamada de glewInit()" << std::endl;
	}

	// Message/Game Loop
	const double FPS = 120.0;
	const double GAMESPEED = 120.0;
	game->running = true;

	// Enable Vsync
	wglSwapIntervalEXT(1);

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

		if (Game::multiplayer && game != nullptr)
		{
			if (Update10Time >= 1.0 / 7)				// Send packets 7 times per second
			{
				Update10Time = 0;
				PacketController::update10();
			}
		}

		if (GameTime >= 1.0 / GAMESPEED)
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
			{
				if (msg.message == WM_QUIT)
					game->running = false;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			game->update();
			game->input();
			linked::Window::updateWindows();

			GameTime = GameTime - (1.0 / GAMESPEED);
		}

		if (RenderTime >= 1.0 / FPS)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Render GUI and game
			game->render();
			linked::Window::renderWindows();
			game->renderGUI();

			SwapBuffers(window->getHDC());

			// Time control
			RenderTime = RenderTime - (1.0 / FPS);
			Frames++;
		}
	}

	delete game;
	delete window;

#if _DEBUG
	fclose(pCout);	// release console
#endif
	return (int)msg.wParam;
}

