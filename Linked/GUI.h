#pragma once
#include "Text.h"
#include <vector>

class Player;
class Mesh;
class Texture;
class Shader;
enum leftGUIAttribs;

#define LEFTGUI_PATH "./res/GUI/Linked_GUI3.png"

class GUI
{
public:
	GUI(Player* player);
	~GUI();

	void render(Shader* shader);
	void update();

private:
	Player* player;
	Mesh* leftGUIMesh;
	Entity* leftGUIEntity;
	Texture* leftGUITexture;
	Texture* fontTexture;

	std::vector<Text*> leftGUIText;

	void initLeftGUI();
	void initLeftGUIText(int attribsHint);

	// Player attribs
	unsigned int playerHealth;
	unsigned int playerMaxHealth;
	unsigned int playerAttack;
	unsigned int playerDefense;
	unsigned int playerMagicalPower;
	unsigned int playerAttackSpeed;
	unsigned int playerSpeed;

	void setPlayerHealth(unsigned int health, unsigned int maxHealth);
	void setPlayerAttack(unsigned int attack);
	void setPlayerDefense(unsigned int defense);
	void setPlayerMagicalPower(unsigned int magicalPower);
	void setPlayerAttackSpeed(unsigned int attackSpeed);
	void setPlayerSpeed(unsigned int speed);

	const float textSize = 0.03f;
	const float yspread = 0.03f;
	const float yoffset = 0.035f;
	const float xoffset = 0.875f;
	
};

enum leftGUIAttribs
{
	GUI_NAME,
	GUI_HEALTH,
	GUI_ATTACK,
	GUI_DEFENSE,
	GUI_MAGICAL_POWER,
	GUI_ATTACK_SPEED,
	GUI_SPEED,
	MAX
};