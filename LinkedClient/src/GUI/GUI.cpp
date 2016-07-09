#include "GUI.h"
#include "Player.h"
#include "SkillIcon.h"
#include "Chat.h"
#include "ContextWindow.h"

#include <sstream>
#include <iostream>

std::vector<std::string> GUI::messages;

linked::Window* GUI::leftGUI;
linked::Window* GUI::chatGUI;
linked::Window* GUI::skillsGUI;

linked::WindowDiv* GUI::messagesDiv;
linked::WindowDiv* GUI::chatDiv;
linked::WindowDiv* GUI::skill1;
linked::WindowDiv* GUI::skill2;
linked::WindowDiv* GUI::skill3;
linked::WindowDiv* GUI::skill4;

GUI::GUI(Player* player)
{
	this->player = player;
	this->playerHealth = player->getHp();
	this->playerMaxHealth = player->getTotalMaximumHp();
	this->playerAttack = player->getTotalAttack();
	this->playerDefense = player->getTotalDefense();
	this->playerMagicalPower = player->getTotalMagicalPower();
	this->playerAttackSpeed = player->getTotalAttackSpeed();
	this->playerSpeed = player->getTotalSpeed();

	this->color = glm::vec3(LGUI_R, LGUI_G, LGUI_B);
	this->activeText = glm::vec3(0.8f, 0.8f, 0.95f);

	currentDisplayWidth = ContextWindow::getCurrent().getWidth();
	currentDisplayHeight = ContextWindow::getCurrent().getHeight();

	initLeftGUI();
	initLeftGUISkills();
}


GUI::~GUI()
{
	// temp code
	skill1->setBackgroundTexture(nullptr);
	skill2->setBackgroundTexture(nullptr);
	skill3->setBackgroundTexture(nullptr);
	skill4->setBackgroundTexture(nullptr);
}

void GUI::initLeftGUI()
{
	using namespace linked;

	// Left GUI Window
	Window* stats = new Window(300, 140, glm::vec2(10, currentDisplayHeight - 150), glm::vec4(0.16f, 0.15f, 0.2f, 0.5f), (unsigned char*)player->getName().c_str(), player->getName().size() + 1,
		W_BORDER | W_MOVABLE | W_HEADER);
	stats->setBorderColor(glm::vec4(0, 0, 0.1f, 0.8f));
	stats->setBorderSizeX(1);
	stats->setBorderSizeY(1);
	stats->setTitleColor(glm::vec4(0.7f, 0.71f, 0.77f, 1.0f));
	stats->setTitleCentered(true);
	leftGUI = stats;

	WindowDiv* playerInfo = new WindowDiv(*stats, 190, 140, 0, 0, glm::vec2(0, 0), glm::vec4(1, 1, 1, 0),
		DIV_ANCHOR_LEFT | DIV_ANCHOR_TOP);
	stats->divs.push_back(playerInfo);
	initLeftGUIText(playerInfo);

	// Chat Window
	Window* chat = new Window(500, 140, glm::vec2(currentDisplayWidth - 510, currentDisplayHeight - 150), glm::vec4(0.16f, 0.15f, 0.2f, 0.5f), (unsigned char*)std::string("Chat").c_str(), 5,
		W_BORDER | W_MOVABLE);
	chat->setBorderColor(glm::vec4(0, 0, 0.1f, 0.6f));
	chat->setBorderSizeX(1);
	chat->setBorderSizeY(1);
	chatGUI = chat;

	chatDiv = new WindowDiv(*chat, 500, 25, 0, 0, glm::vec2(0, 0), glm::vec4(0, 0, 0, 0.3f),
		DIV_ANCHOR_BOTTOM | DIV_ANCHOR_LEFT);
	chat->divs.push_back(chatDiv);

	chatLabel = new Label(*chatDiv, (unsigned char*)chatString.c_str(), chatString.size(), glm::vec2(0, 0), glm::vec4(1, 1, 1, 1), 30, 5, 0);
	chatDiv->getLabels().push_back(chatLabel);

	messagesDiv = new WindowDiv(*chat, 500, 115, 0, 0, glm::vec2(0, 0), glm::vec4(0, 0, 0, 0),
		DIV_ANCHOR_TOP | DIV_ANCHOR_LEFT);
	messagesDiv->m_render = false;
	chat->divs.push_back(messagesDiv);

	for (unsigned int i = 0; i < CHAT_MAX_MSGS; i++)
		messagesDiv->getLabels().push_back(new Label(*messagesDiv, nullptr, 0, glm::vec2(0, 105 - ((i + 1) * 15)), glm::vec4(1, 1, 1, 1), 30, 5, 0));
}

void GUI::initLeftGUIText(linked::WindowDiv* div)
{
	std::stringstream healthStream;
	std::stringstream attackStream;
	std::stringstream defenseStream;
	std::stringstream magicalPowerStream;
	std::stringstream attackSpeedStream;
	std::stringstream speedStream;

	healthStream << "Health: " << playerHealth <<"/"<< playerMaxHealth;
	attackStream << "Attack: " << playerAttack;
	defenseStream << "Defense: " << playerDefense;
	magicalPowerStream << "Magic Power: " << playerMagicalPower;
	attackSpeedStream << "Attack Speed: " << playerAttackSpeed;
	speedStream << "Speed: " << playerSpeed;

	pHealth = healthStream.str();
	pAttack = attackStream.str();
	pDefense = defenseStream.str();
	pMagicalPower = magicalPowerStream.str();
	pAttackSpeed = attackSpeedStream.str();
	pSpeed = speedStream.str();

	using namespace linked;

	glm::vec4 statsTextColor = glm::vec4(0.8f, 0.82f, 0.89f, 1);
	const float statsFontSize = 35;

	healthLabel = new Label(*div, (unsigned char*)pHealth.c_str(), pHealth.size(), glm::vec2(0, 0), statsTextColor, statsFontSize, 10, 0);
	div->getLabels().push_back(healthLabel);
	attackLabel = new Label(*div, (unsigned char*)pAttack.c_str(), pAttack.size(), glm::vec2(0, 20), statsTextColor, statsFontSize, 10, 0);
	div->getLabels().push_back(attackLabel);
	defenseLabel = new Label(*div, (unsigned char*)pDefense.c_str(), pDefense.size(), glm::vec2(0, 40), statsTextColor, statsFontSize, 10, 0);
	div->getLabels().push_back(defenseLabel);
	magicalPowerLabel = new Label(*div, (unsigned char*)pMagicalPower.c_str(), pMagicalPower.size(), glm::vec2(0, 60), statsTextColor, statsFontSize, 10, 0);
	div->getLabels().push_back(magicalPowerLabel);
	attackSpeedLabel = new Label(*div, (unsigned char*)pAttackSpeed.c_str(), pAttackSpeed.size(), glm::vec2(0, 80), statsTextColor, statsFontSize, 10, 0);
	div->getLabels().push_back(attackSpeedLabel);
	speedLabel = new Label(*div, (unsigned char*)pSpeed.c_str(), pSpeed.size(), glm::vec2(0, 100), statsTextColor, statsFontSize, 10, 0);
	div->getLabels().push_back(speedLabel);
}

void GUI::initLeftGUISkills()
{
	using namespace linked;

	int displayHeight = ContextWindow::getCurrent().getHeight();
	Window* skills = new Window(220, 55, glm::vec2(10 + 300 + 10, displayHeight - 65), glm::vec4(0.76f, 0.75f, 0.8f, 0.1f), nullptr, 0, W_BORDER | W_MOVABLE);
	skills->setBorderSizeX(1);
	skills->setBorderSizeY(1);
	skills->setBorderColor(glm::vec4(0, 0, 0, 1));
	skillsGUI = skills;

	const int divWidths = 220 / 4;

	skill1 = new WindowDiv(*skills, divWidths, 55, 0, 0, glm::vec2(0			, 0), glm::vec4(1, 0, 0, 1), DIV_ANCHOR_LEFT | DIV_ANCHOR_TOP);
	skill2 = new WindowDiv(*skills, divWidths, 55, 0, 0, glm::vec2(divWidths * 1, 0), glm::vec4(0, 1, 0, 1), DIV_ANCHOR_LEFT | DIV_ANCHOR_TOP);
	skill3 = new WindowDiv(*skills, divWidths, 55, 0, 0, glm::vec2(divWidths * 2, 0), glm::vec4(1, 0, 1, 1), DIV_ANCHOR_LEFT | DIV_ANCHOR_TOP);
	skill4 = new WindowDiv(*skills, divWidths, 55, 0, 0, glm::vec2(divWidths * 3, 0), glm::vec4(1, 1, 0, 1), DIV_ANCHOR_LEFT | DIV_ANCHOR_TOP);
	
	
	skill1->setBackgroundTexture(player->getSkills()[0]->getSkillIcon()->getTexture());
	skill2->setBackgroundTexture(player->getSkills()[1]->getSkillIcon()->getTexture());
	skill3->setBackgroundTexture(player->getSkills()[2]->getSkillIcon()->getTexture());
	skill4->setBackgroundTexture(player->getSkills()[3]->getSkillIcon()->getTexture());
	
	skills->divs.push_back(skill1);
	skills->divs.push_back(skill2);
	skills->divs.push_back(skill3);
	skills->divs.push_back(skill4);
}

void GUI::render()
{
	chatString = Chat::getStream().str();

	if (Chat::isChatActive())
		chatDiv->setBackgroundColor(glm::vec4(0, 0, 0, 0.3f));
	else
		chatDiv->setBackgroundColor(glm::vec4(0, 0, 0, 0));

	chatLabel->setText((unsigned char*)chatString.c_str(), chatString.size());

}

void GUI::setPlayerHealth(unsigned int health, unsigned int maxHealth)
{
	std::stringstream healthStream;
	healthStream << "Health: " << health << "/" << maxHealth;
	playerHealth = health;
	playerMaxHealth = maxHealth;
	pHealth = healthStream.str();
}
void GUI::setPlayerAttack(unsigned int attack)
{
	std::stringstream attackStream;
	attackStream << "Attack: " << attack;
	playerAttack = attack;
}
void GUI::setPlayerDefense(unsigned int defense)
{
	std::stringstream defenseStream;
	defenseStream << "Defense: " << defense;
	playerDefense = defense;
}
void GUI::setPlayerMagicalPower(unsigned int magicalPower)
{
	std::stringstream magicalPowerStream;
	magicalPowerStream << "Magical Power: " << magicalPower;
	playerMagicalPower = magicalPower;
}
void GUI::setPlayerAttackSpeed(unsigned int attackSpeed)
{
	std::stringstream attackSpeedStream;
	attackSpeedStream << "Attack Speed: " << attackSpeed;
	playerAttackSpeed = attackSpeed;
}
void GUI::setPlayerSpeed(unsigned int speed)
{
	std::stringstream speedStream;
	speedStream << "Speed: " << speed;
	playerSpeed = speed;
}

void GUI::update()
{
	skill1->setBackgroundTexture(player->getSkills()[0]->getSkillIcon()->getTexture());
	skill2->setBackgroundTexture(player->getSkills()[1]->getSkillIcon()->getTexture());
	skill3->setBackgroundTexture(player->getSkills()[2]->getSkillIcon()->getTexture());
	skill4->setBackgroundTexture(player->getSkills()[3]->getSkillIcon()->getTexture());

	if (player->getHp() != playerHealth)
	{
		setPlayerHealth(player->getHp(), player->getTotalMaximumHp());
		healthLabel->setText((unsigned char*)pHealth.c_str(), pHealth.size());
	}
	
	if (player->getTotalAttack() != playerAttack)
	{
		setPlayerAttack(player->getTotalAttack());
		attackLabel->setText((unsigned char*)pAttack.c_str(), pAttack.size());
	}

	if (player->getTotalDefense() != playerDefense)
	{
		setPlayerDefense(player->getTotalDefense());
		defenseLabel->setText((unsigned char*)pDefense.c_str(), pDefense.size());
	}

	if (player->getTotalMagicalPower() != playerMagicalPower)
	{
		setPlayerMagicalPower(player->getTotalMagicalPower());
		magicalPowerLabel->setText((unsigned char*)pMagicalPower.c_str(), pMagicalPower.size());
	}

	if (player->getTotalAttackSpeed() != playerAttackSpeed)
	{
		setPlayerAttackSpeed(player->getTotalAttackSpeed());
		attackSpeedLabel->setText((unsigned char*)pAttackSpeed.c_str(), pAttackSpeed.size());
	}

	if (player->getTotalSpeed() != playerSpeed)
	{
		setPlayerSpeed(player->getTotalSpeed());
		speedLabel->setText((unsigned char*)pSpeed.c_str(), pSpeed.size());
	}
}

void GUI::addSkillIcon(SkillIcon* skillIcon)
{
	SkillSlot slot = skillIcon->getSlot();

	switch (slot)
	{
		case SLOT_1: this->skillIconSlot1 = skillIcon; break;
		case SLOT_2: this->skillIconSlot2 = skillIcon; break;
		case SLOT_3: this->skillIconSlot3 = skillIcon; break;
		case SLOT_4: this->skillIconSlot4 = skillIcon; break;
	}
}

void GUI::renderSkillIcons(Shader* shader)
{
	if (this->skillIconSlot1 != NULL) this->skillIconSlot1->render(shader);
	if (this->skillIconSlot2 != NULL) this->skillIconSlot2->render(shader);
	if (this->skillIconSlot3 != NULL) this->skillIconSlot3->render(shader);
	if (this->skillIconSlot4 != NULL) this->skillIconSlot4->render(shader);
}

void GUI::setNextMessage(std::string& msg)
{
	linked::Label* l = messagesDiv->getLabels()[0];
	if (l->getYAdvance() > 0) 
	{
		for (int i = 0; i < l->getYAdvance(); i++)
		{
			if (messages.size() < CHAT_MAX_MSGS)
			{
				messages.insert(messages.begin(), "");
			}
			else
			{
				messages.erase(messages.begin() + messages.size() - 1);
				messages.insert(messages.begin(), "");
			}
		}
	}

	if (messages.size() < CHAT_MAX_MSGS)
	{
		messages.insert(messages.begin(), msg);
	}
	else
	{
		messages.erase(messages.begin()+messages.size()-1);
		messages.insert(messages.begin(), msg);
	}

	for (unsigned int i = 0; i < messages.size(); i++)
	{
		//linked::Label* l = linked::Window::openedWindows[1]->divs[1]->getLabels()[i];
		linked::Label* l = messagesDiv->getLabels()[i];
		l->setText((unsigned char*)messages[i].c_str(), messages[i].size());
	}

}

void GUI::resizeCallback(int width, int height)
{
	int diffx = currentDisplayWidth - width;
	int diffy = currentDisplayHeight - height;

	leftGUI->setPosition(leftGUI->getPosition() + glm::vec2(diffx / 2.0f, -diffy / 2.0f));
	chatGUI->setPosition(chatGUI->getPosition() + glm::vec2(-diffx / 2.0f, -diffy / 2.0f));
	skillsGUI->setPosition(skillsGUI->getPosition() + glm::vec2(diffx / 2.0f, -diffy / 2.0f));

	currentDisplayWidth = width;
	currentDisplayHeight = height;
}