#pragma once
#include <windows.h>
#include <windowsx.h>
#include <time.h>
#include "GL/glew.h"
#include "native/ContextWindow.h"
#include "Core/Game.h"
#include "Core/LinkedTime.h"
#include "Window.h"
#include "Input.h"
#include "PacketController.h"

#define _DEBUG 1

#ifndef COMMAND_LINE_COMPILE
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")

#ifdef _WIN32
#pragma comment(lib, "./lib/win32/glew32s.lib")
#pragma comment(lib, "./lib/win32/freetype263MT.lib")


#ifdef SFML_STATIC
#pragma comment(lib, "./lib/win32/sfml-audio-2.lib")
#pragma comment(lib, "./lib/win32/sfml-system-2.lib")
#pragma comment(lib, "./lib/win32/flac.lib")
#pragma comment(lib, "./lib/win32/ogg.lib")
#pragma comment(lib, "./lib/win32/openal32.lib")
#pragma comment(lib, "./lib/win32/vorbis.lib")
#pragma comment(lib, "./lib/win32/vorbisenc.lib")
#pragma comment(lib, "./lib/win32/vorbisfile.lib")
#else
#pragma comment(lib, "./lib/win32/sfml-audio.lib")
#endif
#endif

#endif
#include <iostream>
#define LOG(x) std::cout << x << std::endl