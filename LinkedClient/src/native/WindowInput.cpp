#include "WindowInput.h"
#include "ContextWindow.h"
#include "Input.h"

#include <GL/glew.h>
#include <sstream>
#include <iostream>

LRESULT CALLBACK WindowInput::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::stringstream ss;

	auto getLowerCase = [](int c) -> int {
		if (c > 0x40 && c < 0x5A)	// A - Z
			c += 32;
		return c;
	};

	switch (message)
	{
	case WM_MOUSEMOVE:
		std::cout << LOWORD(lParam) << " " << HIWORD(lParam) << std::endl;
		Input::mousePos.x = LOWORD(lParam);
		Input::mousePos.y = HIWORD(lParam);
		Input::MousePosition(LOWORD(lParam), HIWORD(lParam));
		Input::mouseAttack.setMouseCoords(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		Input::leftMouseButton = true;
		SetCapture(ContextWindow::getCurrent().getHandler());
		Input::LeftClick();
		break;
	case WM_LBUTTONUP:
		Input::attack = !Input::attack;
		Input::leftMouseButton = false;
		ReleaseCapture();
		break;
	case WM_KEYDOWN:
		Input::keyStates[getLowerCase(wParam)] = true;
		Input::keyStates[wParam] = true;
		break;
	case WM_KEYUP:
		Input::keyStates[getLowerCase(wParam)] = false;
		Input::keyStates[wParam] = false;
		break;
	case WM_CHAR:
		// TODO: chat
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}