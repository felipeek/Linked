#pragma once
#include "Common.h"
#include <windows.h>
#include <windowsx.h>

class WindowInput
{
public:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};
