#include "Common.h"

#define MULTIPLAYER_TICKRATE 7
#define UPDATE_TICK 60.0

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	ContextWindow* window;
	Game* game;
	MSG msg;

#if _DEBUG
	AllocConsole();
	FILE* pCout;
	freopen_s(&pCout, "CONOUT$", "w", stdout);
#endif

	window = new ContextWindow(nCmdShow, hInstance, 1440, 910, std::string("hoengine_opengl"), std::string("Linked - v2.0"));
	window->InitOpenGL();
	game = new Game(window->getWidth(), window->getHeight());

	glewExperimental = true;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "Erro na chamada de glewInit()" << std::endl;
	}

	game->running = true;

	// Enable Vsync
	wglSwapIntervalEXT(1);

	double StartTime = 0.0;
	double EndTime = 0.0;
	double Elapsed = 0.0;
	double UpdateMultiplayerTick = 0.0;
	double UpdateTick = 60.0;
	double OneSecondCount = 0.0;
	int Frames = 0;

	while (game->running)
	{
		EndTime = LinkedTime::getTime();
		Elapsed = EndTime - StartTime;

		StartTime = LinkedTime::getTime();
		UpdateMultiplayerTick += Elapsed;
		OneSecondCount += Elapsed;

		if (OneSecondCount >= 1.0)
		{
#ifdef _DEBUG
			LOG(Frames);
#endif
			Frames = 0;
			OneSecondCount = 0;
		}

		StartTime = LinkedTime::getTime();

		if (Game::multiplayer && game != nullptr)
		{
			// Send only MULTIPLAYER_TICKRATE packets/s
			if (UpdateMultiplayerTick >= 1.0 / MULTIPLAYER_TICKRATE)
			{
				UpdateMultiplayerTick = 0;
				PacketController::update10();
			}
		}

		if (UpdateTick >= 1.0f / UPDATE_TICK)
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
		}

		game->render();
		linked::Window::renderWindows();
		game->renderGUI();

		SwapBuffers(window->getHDC());

		Frames++;
	}

	delete game;
	delete window;

#if _DEBUG
	fclose(pCout);
#endif

	return (int)msg.wParam;
}

