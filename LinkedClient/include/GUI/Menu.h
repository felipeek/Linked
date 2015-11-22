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
	//static void update(int key, int scancode, int action, int mods);
	static void update();
	static void setStateMenu(int state) { stateMenu = state; }
private:
	static linked::Window* menu;
	static linked::Window* serverIpMenu;
	
	static std::string newSinglePlayerString,
		newMultiPlayerString,
		exitGameString,
		serverIpString,
		defaultIp;

	static bool m_menuActive;
	static int stateMenu;

	static void createMenu();
	static void createServerIpMenu();
	static void deleteWindow(linked::Window* w);
	static void nextState();

	static void singlePlayer();
	static void multiPlayer();
	static void exitGame();
};