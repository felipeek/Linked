#pragma once
#include "Skill.h"
#include <vector>
#include <string>
#include "TextRenderer.h"

class Player;
class Mesh;
class Texture;
class Shader;
enum leftGUIAttribs;

#define LEFTGUI_PATH_CHATINACTIVE "./res/GUI/GUI_Chat_Inactive1.png"
#define LEFTGUI_PATH_CHATACTIVE "./res/GUI/GUI_Chat_Active1.png"
#define NOSKILLICON_PATH "./res/GUI/no_skillicon.png"

#define LGUI_R 0.8f
#define LGUI_G 0.8f
#define LGUI_B 0.8f

#define PLAYER_STATS_FONT_SIZE 0.16f
#define PLAYER_STATS_LEFT_ALIGN 25.0f
#define PLAYER_STATS_VERT_ALIGN 118.0f

#define CHAT_SPACING 14
#define CHAT_LETTER_SIZE 0.15f
#define CHAT_MAX_MSGS 7

class GUI
{
public:
	GUI(Player* player, std::string textShaderFileName, std::string guiShaderFileName, std::string fontName);
	~GUI();

	void render();
	void update();

	void addSkillIcon(SkillIcon* skillIcon);

	void setLeftGUITextColor(glm::vec3& color);
	void setFontSize(int size);

	void setNextMessage(std::string& msg);

	Shader* getGUIShader();
	TextRenderer* getTextRenderer();
private:
	TextRenderer* textRenderer;
	Player* player;
	Mesh* leftGUIMesh;
	Entity* leftGUIEntity;
	Texture* leftGUIChatInactiveTexture;
	Texture* leftGUIChatActiveTexture;
	Shader* textShader;
	Shader* guiShader;

	// Left GUI
	std::vector<Text*> leftGUIText;
	std::vector<std::string> messages;
	glm::vec3 color;
	glm::vec3 activeText;
	int fontSize;

	void initLeftGUI();
	void initLeftGUIText(int attribsHint);
	void initLeftGUISkills();

	// Player attribs
	unsigned int playerHealth;
	unsigned int playerMaxHealth;
	unsigned int playerAttack;
	unsigned int playerDefense;
	unsigned int playerMagicalPower;
	unsigned int playerAttackSpeed;
	unsigned int playerSpeed;

	std::string pHealth;
	std::string pAttack;
	std::string pDefense;
	std::string pMagicalPower;
	std::string pAttackSpeed;
	std::string pSpeed;

	void setPlayerHealth(unsigned int health, unsigned int maxHealth);
	void setPlayerAttack(unsigned int attack);
	void setPlayerDefense(unsigned int defense);
	void setPlayerMagicalPower(unsigned int magicalPower);
	void setPlayerAttackSpeed(unsigned int attackSpeed);
	void setPlayerSpeed(unsigned int speed);	

	// Skill Icons
	SkillIcon* skillIconSlot1;
	SkillIcon* skillIconSlot2;
	SkillIcon* skillIconSlot3;
	SkillIcon* skillIconSlot4;

	void renderSkillIcons(Shader* shader);
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