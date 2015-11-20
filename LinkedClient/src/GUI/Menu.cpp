#include "Menu.h"
#include "Display.h"
#include <vector>
#include <iostream>

bool Menu::m_menuActive = false;
int Menu::stateMenu = 0;
linked::Window* Menu::menu;

std::string Menu::pausedString = std::string("Paused");

void Menu::nextState()
{
	if (stateMenu == 3)
		stateMenu = 0;
	else
		stateMenu++;
}

void Menu::update(int key, int scancode, int action, int mods)
{
	if ((key == ESCAPE_KEY && action == 0) || (key == ESCAPE_KEY && action == 1))
		Menu::nextState();

	if (stateMenu == MENU_ACTIVE)
	{
		m_menuActive = true;
		createMenu();
	}
	if (stateMenu == NORMAL_INACTIVE)
	{
		m_menuActive = false;
		deleteMenu();
	}
}

void Menu::createMenu()
{
	if (menu == nullptr)
	{
		const float menuWidth = Display::getCurrentInstance().getWidth();
		const float menuHeight = Display::getCurrentInstance().getHeight();

		menu = new linked::Window(menuWidth, menuHeight, glm::vec2(0, 0), glm::vec4(0, 0, 0, 0.7f), nullptr, 0, linked::W_BORDER);
		linked::WindowDiv* div = new linked::WindowDiv(*menu, 270, 100, 0, 0, glm::vec2(0, 0), glm::vec4(1, 1, 1, 1), linked::DIV_CENTER_X | linked::DIV_CENTER_Y);
		div->m_render = false;
		menu->divs.push_back(div);
		linked::Label* pauseLabel = new linked::Label(*div, (unsigned char*)pausedString.c_str(), pausedString.size(), 150);
		pauseLabel->setTextColor(glm::vec4(1, 1, 1, 0.9f));
		div->getLabels().push_back(pauseLabel);
	}
}

void Menu::deleteMenu()
{
	for (unsigned int i = 0; i < linked::Window::openedWindows.size(); i++)
	{
		if (linked::Window::openedWindows[i] == menu)
			linked::Window::openedWindows.erase(linked::Window::openedWindows.begin() + i);
	}
	if (menu != nullptr)
	{
		delete menu;
		menu = nullptr;
	}
}