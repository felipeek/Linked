#pragma once
#include "Common.h"
#include <glm/glm.hpp>

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include <string>

class ContextWindow
{
public:
	ContextWindow(int nCmdShow, HINSTANCE hInstance,
		int width, int height, std::string className,
		std::string windowTitle);
	~ContextWindow();
	void InitOpenGL();

private: // Data
	int m_width, m_height;

	std::string m_windowClass;
	std::string m_title;
	HINSTANCE m_hInst;
	WNDCLASSEX m_wcex;

	HDC m_hDC;
	HGLRC m_hRC;

	HWND m_hWnd;

	static ContextWindow* current;
private: // Methods
	ContextWindow();

public:	// Getters and Setters
	inline HWND getHandler() const { return m_hWnd; }
	inline HDC getHDC() const { return m_hDC; }
	inline static const ContextWindow& getCurrent() { return *current; }
	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }
	glm::vec2& getCursorPosition() const { return glm::vec2(0, 0); }	// TODO: implement
	glm::vec4& getClearColor() const { return glm::vec4(0, 0, 0, 1); }	// TODO: implement
	static void startGame() {}	// TODO: fix
	static void exitGame() {}	// TODO: fix
};

