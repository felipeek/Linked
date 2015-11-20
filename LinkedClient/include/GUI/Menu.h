#pragma once

#include <Window.h>

#include <string>

/* MENU STATEs */
#define NORMAL_INACTIVE 0
#define NORMAL_ACTIVE 1
#define MENU_ACTIVE 2
#define MENU_INACTIVE 3

#define ESCAPE_KEY 256

class Menu
{
public:
	static bool isMenuActive(){ return m_menuActive; }
	static void update(int key, int scancode, int action, int mods);
private:
	static linked::Window* menu;
	
	static std::string pausedString;

	static bool m_menuActive;
	static int stateMenu;

	static void createMenu();
	static void deleteMenu();
	static void nextState();
};