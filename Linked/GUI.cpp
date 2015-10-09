#include "GUI.h"
#include "Player.h"
#include "Shader.h"
#include "SkillIcon.h"
#include "TextShader.h"
#include "Primitive.h"
#include "GUIShader.h"

#include <sstream>
#include <iostream>

GUI::GUI(Player* player, std::string textShaderFileName, std::string guiShaderFileName, std::string fontName)
{
	this->player = player;
	this->playerHealth = player->getHp();
	this->playerMaxHealth = player->getTotalMaximumHp();
	this->playerAttack = player->getTotalAttack();
	this->playerDefense = player->getTotalDefense();
	this->playerMagicalPower = player->getTotalMagicalPower();
	this->playerAttackSpeed = player->getTotalAttackSpeed();
	this->playerSpeed = player->getTotalSpeed();

	this->textShader = new TextShader(textShaderFileName);
	this->guiShader = new GUIShader(guiShaderFileName);

	this->color = glm::vec3(LGUI_R, LGUI_G, LGUI_B);

	textRenderer = new TextRenderer(textShader, fontName);
	initLeftGUI();
}


GUI::~GUI()
{

}

void GUI::initLeftGUI()
{
	leftGUIMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f));
	leftGUITexture = new Texture(LEFTGUI_PATH);
	leftGUIEntity = new Entity(new Transform(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)), leftGUIMesh, leftGUITexture);
	initLeftGUIText(1);
	initLeftGUISkills();
}

void GUI::initLeftGUIText(int attribsHint)
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
}

void GUI::initLeftGUISkills()
{

}

void GUI::render()
{
	leftGUIEntity->render(guiShader);
	renderSkillIcons(guiShader);

	// Render Text
	// Player Name
	textRenderer->renderText(player->getName(), 105.0f, 115.0f, 0.2f, color);
	// Attributes
	textRenderer->renderText(pHealth, 105.0f, 95.0f, 0.18f, color);
	textRenderer->renderText(pAttack, 105.0f, 80.0f, 0.18f, glm::vec3(1,0,0));
	textRenderer->renderText(pDefense, 105.0f, 65.0f, 0.18f, color);
	textRenderer->renderText(pMagicalPower, 105.0f, 50.0f, 0.18f, color);
	textRenderer->renderText(pAttackSpeed, 105.0f, 35.0f, 0.18f, color);
	textRenderer->renderText(pSpeed, 105.0f, 20.0f, 0.18f, color);
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
	if (player->getHp() != playerHealth)
		setPlayerHealth(player->getHp(), player->getTotalMaximumHp());
	
	if (player->getTotalAttack() != playerAttack)
		setPlayerAttack(player->getTotalAttack());

	if (player->getTotalDefense() != playerDefense)
		setPlayerDefense(player->getTotalDefense());

	if (player->getTotalMagicalPower() != playerMagicalPower)
		setPlayerMagicalPower(player->getTotalMagicalPower());

	if (player->getTotalAttackSpeed() != playerAttackSpeed)
		setPlayerAttackSpeed(player->getTotalAttackSpeed());

	if (player->getTotalSpeed() != playerSpeed)
		setPlayerSpeed(player->getTotalSpeed());
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

void GUI::setLeftGUITextColor(glm::vec3& color)
{
	this->color = color;
}

void GUI::setFontSize(int size)
{
	this->fontSize = size;
}

Shader* GUI::getGUIShader()
{
	return this->guiShader;
}

TextRenderer* GUI::getTextRenderer()
{
	return this->textRenderer;
}