#include "GUI.h"
#include "Player.h"
#include "Mesh.h"
#include "Primitive.h"
#include "Texture.h"
#include "Shader.h"

#include <sstream>
#include <string>
#include <iostream>

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

	this->fontTexture = new Texture("./res/Fonts/fontLinkedFinal.png", -10);

	initLeftGUI();
}


GUI::~GUI()
{
	if (leftGUIMesh)
		delete leftGUIMesh;
	if (leftGUIEntity)
		delete leftGUIEntity;
	if (leftGUITexture)
		delete leftGUITexture;
	for (int i = 0; i < leftGUIText.size(); i++)
	{
		delete leftGUIText[i];
		leftGUIText.erase(leftGUIText.begin() + i);
	}
}

void GUI::initLeftGUI()
{
	leftGUIMesh = new Mesh(new Quad(glm::vec3(0, 0, 0), 1.0f, 1.0f));
	leftGUITexture = new Texture(LEFTGUI_PATH);
	leftGUIEntity = new Entity(new Transform(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)), leftGUIMesh, leftGUITexture);
	initLeftGUIText(1);
}

void GUI::initLeftGUIText(int attribsHint)
{
	float yspreadVar = yspread;

	Text* name = new Text(player->getName(), textSize, xoffset, 0.733f, fontTexture);

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

	Text* health = new Text(healthStream.str(), 0.023f, xoffset, 0.733f + yspreadVar + yoffset, fontTexture);
	yspreadVar += yspread;
	Text* attack = new Text(attackStream.str(), 0.023f, xoffset, 0.733f + yspreadVar + yoffset, fontTexture);
	yspreadVar += yspread;
	Text* defense = new Text(defenseStream.str(), 0.023f, xoffset, 0.733f + yspreadVar + yoffset, fontTexture);
	yspreadVar += yspread;
	Text* magpower = new Text(magicalPowerStream.str(), 0.023f, xoffset, 0.733f + yspreadVar + yoffset, fontTexture);
	yspreadVar += yspread;
	Text* attspeed = new Text(attackSpeedStream.str(), 0.023f, xoffset, 0.733f + yspreadVar + yoffset, fontTexture);
	yspreadVar += yspread;
	Text* speed = new Text(speedStream.str(), 0.023f, xoffset, 0.733f + yspreadVar + yoffset, fontTexture);

	leftGUIText.push_back(name);
	leftGUIText.push_back(health);
	leftGUIText.push_back(attack);
	leftGUIText.push_back(defense);
	leftGUIText.push_back(magpower);
	leftGUIText.push_back(attspeed);
	leftGUIText.push_back(speed);

}

void GUI::render(Shader* shader)
{
	leftGUIEntity->render(shader);
	for (Text* t : leftGUIText)
	{
		for (Entity* e : t->getEntities())
		{
			try{
				e->render(shader);
			}
			catch (...){
				std::cerr << "Error rendering entity" << std::endl;
			}
		}
	}	
}

void GUI::setPlayerHealth(unsigned int health, unsigned int maxHealth)
{
	float yspreadVar = (float)GUI_HEALTH * yspread;
	std::stringstream healthStream;
	healthStream << "Health: " << health << "/" << maxHealth;
	delete leftGUIText[GUI_HEALTH];
	Text* healthText = new Text(healthStream.str(), 0.023f, xoffset, 0.733f + yspreadVar + yoffset, fontTexture);
	leftGUIText[GUI_HEALTH] = healthText;
	playerHealth = health;
	playerMaxHealth = maxHealth;
}
void GUI::setPlayerAttack(unsigned int attack)
{
	float yspreadVar = (float)GUI_ATTACK * yspread;
	std::stringstream attackStream;
	attackStream << "Attack: " << attack;
	delete leftGUIText[GUI_ATTACK];
	Text* attackText = new Text(attackStream.str(), 0.023f, xoffset, 0.733f + yspreadVar + yoffset, fontTexture);
	leftGUIText[GUI_ATTACK] = attackText;
	playerAttack = attack;
}
void GUI::setPlayerDefense(unsigned int defense)
{
	float yspreadVar = (float)GUI_DEFENSE * yspread;
	std::stringstream defenseStream;
	defenseStream << "Defense: " << defense;
	delete leftGUIText[GUI_DEFENSE];
	Text* defenseText = new Text(defenseStream.str(), 0.023f, xoffset, 0.733f + yspreadVar + yoffset, fontTexture);
	leftGUIText[GUI_DEFENSE] = defenseText;
	playerDefense = defense;
}
void GUI::setPlayerMagicalPower(unsigned int magicalPower)
{
	float yspreadVar = (float)GUI_MAGICAL_POWER * yspread;
	std::stringstream magicalPowerStream;
	magicalPowerStream << "Defense: " << magicalPower;
	delete leftGUIText[GUI_MAGICAL_POWER];
	Text* magicalPowerText = new Text(magicalPowerStream.str(), 0.023f, xoffset, 0.733f + yspreadVar + yoffset, fontTexture);
	leftGUIText[GUI_MAGICAL_POWER] = magicalPowerText;
	playerMagicalPower = magicalPower;
}
void GUI::setPlayerAttackSpeed(unsigned int attackSpeed)
{
	float yspreadVar = (float)GUI_ATTACK_SPEED * yspread;
	std::stringstream attackSpeedStream;
	attackSpeedStream << "Attack Speed: " << attackSpeed;
	delete leftGUIText[GUI_ATTACK_SPEED];
	Text* attackSpeedText = new Text(attackSpeedStream.str(), 0.023f, xoffset, 0.733f + yspreadVar + yoffset, fontTexture);
	leftGUIText[GUI_ATTACK_SPEED] = attackSpeedText;
	playerAttackSpeed = attackSpeed;
}
void GUI::setPlayerSpeed(unsigned int speed)
{
	float yspreadVar = (float)GUI_SPEED * yspread;
	std::stringstream speedStream;
	speedStream << "Speed: " << speed;
	delete leftGUIText[GUI_ATTACK_SPEED];
	Text* speedText = new Text(speedStream.str(), 0.023f, xoffset, 0.733f + yspreadVar + yoffset, fontTexture);
	leftGUIText[GUI_ATTACK_SPEED] = speedText;
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