#include "Display.h"
#include <time.h>
#include <stdlib.h>

#include <vld.h>

int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));
	Display* display = new Display(&argc, argv, "Linked");
	delete display;
	return 0;
}