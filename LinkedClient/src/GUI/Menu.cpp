#include "Menu.h"
#include "Display.h"
#include "Chat.h"
#include "Game.h"
#include <vector>
#include <iostream>

bool Menu::m_menuActive = true;
bool Menu::multiplayerCanceled = false;
int Menu::stateMenu = 0;
linked::Window* Menu::menu = nullptr;
linked::Window* Menu::serverIpMenu = nullptr;

std::string Menu::newSinglePlayerString = std::string("Single Player");
std::string Menu::newMultiPlayerString = std::string("Multi Player");
std::string Menu::exitGameString = std::string("Exit");
std::string Menu::serverIpString = std::string("Type the server IP address and port");
std::string Menu::defaultIp = std::string("localhost");
std::string Menu::defaultPort = std::string("9090");
std::string Menu::confirmString = std::string("Ok");
std::string Menu::cancelString = std::string("Cancel");

void Menu::nextState()
{
	if (stateMenu == 3)
		stateMenu = 0;
	else
		stateMenu++;
}
/*
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
}*/

void Menu::update()
{
	if (stateMenu == MENU_ACTIVE && menu == nullptr)
	{
		m_menuActive = true;
		createMenu();
	}
	if (stateMenu == NORMAL_INACTIVE)
	{
		m_menuActive = false;
		deleteWindow(menu);
		if (menu != nullptr)
		{
			delete menu;
			menu = nullptr;
		}
		if (serverIpMenu != nullptr)
		{
			deleteWindow(serverIpMenu);
			delete serverIpMenu;
			serverIpMenu = nullptr;
		}
	}
	if (multiplayerCanceled)
	{
		if (serverIpMenu != nullptr)
		{
			deleteWindow(serverIpMenu);
			delete serverIpMenu;
			serverIpMenu = nullptr;
		}
		multiplayerCanceled = false;
	}
}

void Menu::createMenu()
{
	if (menu == nullptr)
	{
		const float menuWidth = Display::getCurrentInstance().getWidth();
		const float menuHeight = Display::getCurrentInstance().getHeight();

		// Menu Window
		menu = new linked::Window(menuWidth, menuHeight, glm::vec2(0, 0), glm::vec4(0.043, 0.045f, 0.05f, 1), nullptr, 0, linked::W_BORDER);

		// Menu div
		linked::WindowDiv* div = new linked::WindowDiv(*menu, 400, 170, 0, 0, glm::vec2(0, 0), glm::vec4(1, 0, 0, 1), linked::DIV_CENTER_X | linked::DIV_CENTER_Y);
		div->m_render = false;
		menu->divs.push_back(div);

		// Menu labels
		linked::Label* newSinglePlayerGameLabel = new linked::Label(*div, (unsigned char*)newSinglePlayerString.c_str(), newSinglePlayerString.size(), 100);
		newSinglePlayerGameLabel->setTextColor(glm::vec4(1, 1, 1, 0.7f));
		
		linked::Label* newMultiplayerGameLabel = new linked::Label(*div, (unsigned char*)newMultiPlayerString.c_str(), newMultiPlayerString.size(), 100);
		newMultiplayerGameLabel->setTextColor(glm::vec4(1, 1, 1, 0.7f));

		linked::Label* exitGameLabel = new linked::Label(*div, (unsigned char*)exitGameString.c_str(), exitGameString.size(), 100);
		exitGameLabel->setTextColor(glm::vec4(1, 1, 1, 0.7f));

		linked::Button* newSinglePlayerGameButton = new linked::Button(*div, newSinglePlayerGameLabel, glm::vec2(0, 0), 380, 50, glm::vec4(1, 0, 0, 0.0f));
		newSinglePlayerGameButton->setHoveredTextColor(glm::vec4(1, 1, 1, 0.9f));
		newSinglePlayerGameButton->setHeldText(glm::vec4(1, 1, 1, 0.7f));
		newSinglePlayerGameButton->setClickedCallback(singlePlayer);
		div->getButtons().push_back(newSinglePlayerGameButton);

		linked::Button* newMultiPlayerGameButton = new linked::Button(*div, newMultiplayerGameLabel, glm::vec2(0, 60), 380, 50, glm::vec4(1, 1, 0, 0.0f));
		newMultiPlayerGameButton->setHoveredTextColor(glm::vec4(1, 1, 1, 0.9f));
		newMultiPlayerGameButton->setHeldText(glm::vec4(1, 1, 1, 0.7f));
		newMultiPlayerGameButton->setClickedCallback(multiPlayer);
		div->getButtons().push_back(newMultiPlayerGameButton);

		linked::Button* exitGameButton = new linked::Button(*div, exitGameLabel, glm::vec2(0, 120), 380, 50, glm::vec4(1, 1, 0, 0.0f));
		exitGameButton->setHoveredTextColor(glm::vec4(1, 1, 1, 0.9f));
		exitGameButton->setHeldText(glm::vec4(1, 1, 1, 0.7f));
		exitGameButton->setClickedCallback(exitGame);
		div->getButtons().push_back(exitGameButton);
		
	}
}

void Menu::createServerIpMenu()
{
	const float menuWidth = Display::getCurrentInstance().getWidth() / 2.0f;
	const float menuHeight = Display::getCurrentInstance().getHeight() / 2.0f;
	// Menu Window
	serverIpMenu = new linked::Window(500, 200, glm::vec2(menuWidth - 250, menuHeight - 100), glm::vec4(0.043, 0.045f, 0.05f, 0.95f), (unsigned char*)serverIpString.c_str(), serverIpString.size() + 1, 
		linked::W_BORDER | linked::W_MOVABLE | linked::W_HEADER);
	serverIpMenu->setBorderColor(glm::vec4(0.89f, 0.88f, 0.9f, 1));
	serverIpMenu->setTitleCentered(false);
	// Menu div
	linked::WindowDiv* div = new linked::WindowDiv(*serverIpMenu, 480, 50, 0, 0, glm::vec2(0, 10), glm::vec4(1, 0, 0, 1), linked::DIV_CENTER_X | linked::DIV_ANCHOR_TOP);
	div->m_render = false;
	serverIpMenu->divs.push_back(div);

	// Menu labels
	linked::Label* ipAddress = new linked::Label(*div, (unsigned char*)defaultIp.c_str(), defaultIp.size(), 100);
	ipAddress->setTextColor(glm::vec4(1, 1, 1, 0.7f));
	div->getLabels().push_back(ipAddress);

	linked::Label* portLabel = new linked::Label(*div, (unsigned char*)defaultPort.c_str(), defaultPort.size(), glm::vec2(0, 60), 100);
	portLabel->setTextColor(glm::vec4(1, 1, 1, 0.7f));
	div->getLabels().push_back(portLabel);

	// Cancel and confirm labels
	linked::Label* confirmLabel = new linked::Label(*div, (unsigned char*)confirmString.c_str(), confirmString.size(), glm::vec2(70, 0), 100);
	confirmLabel->setTextColor(glm::vec4(1, 1, 1, 0.7f));

	linked::Label* cancelLabel = new linked::Label(*div, (unsigned char*)cancelString.c_str(), cancelString.size(), glm::vec2(20, 0), 100);
	confirmLabel->setTextColor(glm::vec4(1, 1, 1, 0.7f));

	// Buttons
	linked::Button* confirmButton = new linked::Button(*div, confirmLabel, glm::vec2(0, 130), 200, 50, glm::vec4(1, 1, 1, 0.5f));
	confirmButton->setHoveredTextColor(glm::vec4(1, 1, 1, 0.9f));
	confirmButton->setHeldText(glm::vec4(1, 1, 1, 0.7f));
	confirmButton->setHoveredBGColor(glm::vec4(1, 1, 1, 0.4f));
	confirmButton->setClickedCallback(startMultiplayer);
	div->getButtons().push_back(confirmButton);

	linked::Button* cancelButton = new linked::Button(*div, cancelLabel, glm::vec2(280, 130), 200, 50, glm::vec4(1, 1, 1, 0.5f));
	cancelButton->setNormalTextColor(glm::vec4(1, 1, 1, 0.7f));
	cancelButton->setHoveredTextColor(glm::vec4(1, 1, 1, 0.9f));
	cancelButton->setHeldText(glm::vec4(1, 1, 1, 0.7f));
	cancelButton->setHoveredBGColor(glm::vec4(1, 1, 1, 0.4f));
	cancelButton->setClickedCallback(cancelMultiplayer);
	div->getButtons().push_back(cancelButton);
}

void Menu::deleteWindow(linked::Window* w)
{
	for (unsigned int i = 0; i < linked::Window::openedWindows.size(); i++)
	{
		if (linked::Window::openedWindows[i] == w)
			linked::Window::openedWindows.erase(linked::Window::openedWindows.begin() + i);
	}
}

void Menu::singlePlayer()
{
	Game::multiplayer = false;
	startSinglePlayer();
}

void Menu::startSinglePlayer()
{
	Display::startGame();
	stateMenu = NORMAL_INACTIVE;
}

void Menu::multiPlayer()
{
	Game::multiplayer = true;
	if (serverIpMenu != nullptr)
	{
		deleteWindow(serverIpMenu);
		delete serverIpMenu;
		serverIpMenu = nullptr;
	}
	createServerIpMenu();
}

void Menu::startMultiplayer()
{
	Display::startGame();
	stateMenu = NORMAL_INACTIVE;
}

void Menu::cancelMultiplayer()
{
	multiplayerCanceled = true;
}

void Menu::exitGame()
{
	Display::exitGame();
}