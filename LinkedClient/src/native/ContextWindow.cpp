#include "ContextWindow.h"
#include "WindowInput.h"
#include "Window.h"

#include <GL/glew.h>
#include "../resource.h"

ContextWindow* ContextWindow::current;

ContextWindow::ContextWindow(int nCmdShow, HINSTANCE hInstance,
	int width, int height, std::string className, std::string windowTitle)
	: m_hInst(hInstance), m_windowClass(className), m_title(windowTitle),
	m_width(width), m_height(height)
{
	// Register Class
	m_wcex.cbSize = sizeof(WNDCLASSEX);
	m_wcex.style = CS_HREDRAW | CS_VREDRAW;
	m_wcex.lpfnWndProc = WindowInput::WndProc;
	m_wcex.cbClsExtra = 0;
	m_wcex.cbWndExtra = 0;
	m_wcex.hInstance = hInstance;
	m_wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(GLFW_ICON));
	m_wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	m_wcex.lpszMenuName = NULL;
	m_wcex.lpszClassName = m_windowClass.c_str();
	m_wcex.hIconSm = LoadIcon(m_wcex.hInstance, MAKEINTRESOURCE(IDI_ERROR));

	if (!RegisterClassEx(&m_wcex))
	{
		LOG("Erro ao criar classe Win32.");
	}
	// adjust size of window
	RECT wr = { 0, 0, width, height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	m_hWnd = CreateWindowEx(
		NULL,
		m_windowClass.c_str(),
		m_title.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL, NULL, m_hInst, NULL
		);

	if (!m_hWnd)
	{
		LOG("Erro ao criar window context");
	}
	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	ContextWindow::current = this;
}
ContextWindow::~ContextWindow()
{

}

void ContextWindow::InitOpenGL()
{
	int PixelFormat;
	m_hDC = GetDC(m_hWnd);

	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pfd.cDepthBits = 32;
	pfd.cColorBits = 24;
	pfd.iPixelType = PFD_TYPE_RGBA;

	PixelFormat = ChoosePixelFormat(m_hDC, &pfd);
	if (!SetPixelFormat(m_hDC, PixelFormat, &pfd))
	{
		LOG("Erro ao criar context pixel descriptor.");
	}

	m_hRC = wglCreateContext(m_hDC);		// Create the context
	wglMakeCurrent(m_hDC, m_hRC);			// Make the context current

	glewExperimental = true;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		LOG("Erro ao inicializar GLEW!");
	}

	setClearColor(glm::vec4(1, 1, 1, 1));
	glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	linked::Window::linkedWindowInit();
}