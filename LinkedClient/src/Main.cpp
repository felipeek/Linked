#include "Display.h"
#include <time.h>

//#include <vld.h>

#if _MSC_VER < 1900
// VC 2013
#pragma comment(lib, "./lib/vc2013/glew32s_vc2013.lib")
#pragma comment(lib, "./lib/vc2013/freetype261_vc2013.lib")
#elif _MSC_VER < 2000
// VC 2015
#pragma comment(lib, "./lib/vc2013/glew32s_vc2015.lib")
#pragma comment(lib, "./lib/vc2013/freetype261_vc2015.lib")
#endif
#pragma comment(lib, "./lib/glfw3.lib")
#pragma comment(lib, "opengl32.lib")

int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));
	Display* display = new Display(&argc, argv, "Linked", 1600, 900);
	delete display;
	return 0;
}

