#include "WindowInput.h"
#include "Input.h"

#include <GL/glew.h>
#include <sstream>
#include <iostream>

LRESULT CALLBACK WindowInput::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::stringstream ss;

	switch (message)
	{
	case WM_MOUSEMOVE:
		std::cout << LOWORD(lParam) << " " << HIWORD(lParam) << std::endl;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		Input::mouseDown = true;
		Input::mousePos = Input::GetMousePos();
		SetCapture(Window::getCurrent().getHandler());
		break;
	case WM_LBUTTONUP:
		Input::mouseDown = false;
		ReleaseCapture();
		break;
	case WM_KEYDOWN:
		Input::keyStates[wParam] = true;
		break;
	case WM_KEYUP:
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