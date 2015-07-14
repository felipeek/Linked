#include "Display.h"
#include <time.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	srand(time(NULL));
	Display* display = new Display(&argc, argv, "Linked");
	return 0;
}