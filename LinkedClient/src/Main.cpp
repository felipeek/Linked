#include "Display.h"
#include <time.h>
#include "glm\glm.hpp"
//#include <vld.h>

int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));
	Display* display = new Display(&argc, argv, "Linked");
	delete display;
	return 0;
}

