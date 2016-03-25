#include "Input.h"
#include "ContextWindow.h"
#include "Window.h"

bool Input::keyStates[KEYSTATES] = { 0 };
int Input::wheel = 0;
bool Input::attack = false;
bool Input::leftMouseButton = false;	
bool Input::rightMouseButton = false;
MouseAttack Input::mouseAttack;
glm::vec2 Input::mousePos;

void Input::clear()
{
	for (int i = 0; i < KEYSTATES; i++)
	{
		keyStates[i] = 0;
	}
	wheel = 0;
	attack = false;
}

void Input::LeftClick()
{
	int width = ContextWindow::getCurrent().getWidth();
	int height = ContextWindow::getCurrent().getHeight();

	const float aspect = (float)width / height;
	float screenX = ((float)mousePos.x * 2 / width - 1.0f);
	float screenY = -((float)mousePos.x * 2 / height - 1.0f);

	std::cout << screenX << " " << screenY << std::endl;

	Input::attack = !Input::attack;
	Input::mouseAttack.setAttackPos(screenX, screenY);
	Input::mouseAttack.setMouseCoords((int)mousePos.x, (int)mousePos.y);

	for (unsigned int i = 0; i < linked::Window::openedWindows.size(); i++)
		linked::Window::openedWindows[i]->mouseCallback(0, 1, 0);
	linked::Button::mouseCallback(0, 1, 0);
}

void Input::MousePosition(int x, int y)
{
	int width = ContextWindow::getCurrent().getWidth();
	int height = ContextWindow::getCurrent().getHeight();

	const float aspect = (float)width / height;
	float screenX = ((float)x * 2 / width - 1.0f);
	float screenY = -((float)y * 2 / height - 1.0f);

	Input::mouseAttack.setAttackPos(screenX, screenY);
	Input::mouseAttack.setMouseCoords(x, y);

	ContextWindow::getCurrent().setCursorPosition(glm::vec2(x, y));
}