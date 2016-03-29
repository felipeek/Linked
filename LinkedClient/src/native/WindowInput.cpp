#include "WindowInput.h"
#include "ContextWindow.h"
#include "Input.h"
#include "Game.h"
#include "GUI.h"
#include "Chat.h"

#include <GL/glew.h>
#include <sstream>
#include <iostream>

LRESULT CALLBACK WindowInput::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::stringstream ss;

	auto getLowerCase = [](int c) -> int
	{
		if (c > 0x40 && c < 0x5A)	// A - Z
			c += 32;
		return c;
	};
	auto isExtendedKey = [](int key) -> bool 
	{
		// if 24th bit is 1 then the key is extended
		// Alt or ctrl key
		if ((key & 0x1000000) != 0)
			return true;
		return false;
	};

	switch (message)
	{
	case WM_MOUSELEAVE:
		break;
	case WM_MOUSEMOVE:
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
		Input::LeftClickRelease();
		ReleaseCapture();
		break;
	case WM_RBUTTONDOWN:
		Input::rightMouseButton = true;
		break;
	case WM_RBUTTONUP:
		Input::rightMouseButton = false;
		break;
	case WM_KEYDOWN:
		Input::keyStates[wParam] = true;
		// Ctrl+V to paste in chat
		if (Input::keyStates[17] && Input::keyStates['V'])
		{
			if(Chat::isChatActive())
				Chat::getStream() << Input::GetClipboardText();
		}
		break;
	case WM_KEYUP:
		Input::keyStates[wParam] = false;
		break;
	case WM_SETCURSOR:
	{
		WORD ht = LOWORD(lParam);
		static bool hiddencursor = false;
		if (HTCLIENT == ht && !hiddencursor)
		{
			hiddencursor = true;
			ShowCursor(false);
		}
		else if (HTCLIENT != ht && hiddencursor)
		{
			hiddencursor = false;
			ShowCursor(true);
		}
	}
	break;
	case WM_MOUSEWHEEL:		
		Input::wheel = static_cast<short>(HIWORD(wParam));
		break;
	case WM_CHAR:
		if(!Input::keyStates[17])
			Chat::ProcessKeyStroke(wParam);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}