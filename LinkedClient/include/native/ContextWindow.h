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
	glm::vec4 m_ClearColor;
	glm::vec2 m_CursorPos;

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
	inline void setClearColor(glm::vec4& color) { m_ClearColor = color; }
	inline glm::vec4 getClearColor() const { return m_ClearColor; }
	inline HWND getHandler() const { return m_hWnd; }
	inline HDC getHDC() const { return m_hDC; }
	inline static ContextWindow& getCurrent() { return *current; }
	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }
	const glm::vec2& getCursorPosition() const { return m_CursorPos; }
	void setCursorPosition(glm::vec2& pos) { m_CursorPos = pos; }
	static void startGame() {}	// TODO: fix
	static void exitGame() {}	// TODO: fix
	inline bool isWindowFocused() const { return GetFocus() == m_hWnd; }
	inline void setWindowSize(int width, int height) { m_width = width, m_height = height; }
};

