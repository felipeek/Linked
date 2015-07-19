#include "HPBar.h"
#include "Texture.h"
#include "Player.h"
#include "Entity.h"
#include "Primitive.h"
#include "Input.h"


HPBar::HPBar(Player* player)
{
	this->player = player;
	this->maxBars = NUMBARS;
	this->numBars = NUMBARS;						// Start with full hp

	hpBar = new DynamicTexture(maxBars, 1, 1, 0, false);		// Create Texture
	hpBarImage = new unsigned char[4 * maxBars];	// Allocate memory for image
	setHP(maxBars);
	hpBar->setLoadedImage(hpBarImage);				// Set loaded image address
	hpBar->genDynamicGLTexture(true);

	maxHP = player->getTotalMaximumHp();

	Mesh* mesh = new Mesh(new Quad(glm::vec3(0, -0.5, 0), 0.5f, 0.1f));
	Transform* transform = new Transform(
		glm::vec3(0, 0, 0), 30, glm::vec3(1, 0, 0), glm::vec3(3,3,3)
		);
	quad = new Entity(transform, mesh, hpBar);
}


HPBar::~HPBar()
{
	if (hpBarImage)
		delete[] hpBarImage;
	if (quad)
		delete quad;
}

void HPBar::update()
{
	glm::vec3 playerPos = player->getTransform()->getPosition();
	quad->getTransform()->translate(playerPos.x, playerPos.y-1, playerPos.z+1);
	
	unsigned int currentHP = player->getHp();
	float percentHP = (float)currentHP / (float)player->getTotalMaximumHp();
	numBars = (unsigned int)(percentHP * maxBars);

	if (prevBars != numBars)
	{
		prevBars = numBars;
		setHP(numBars);
	}
	
	glBindTexture(GL_TEXTURE_2D, hpBar->textureID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, maxBars, 1, GL_RGBA, GL_UNSIGNED_BYTE, hpBarImage);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 100, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, hpBarImage);
}

void HPBar::setHP(unsigned int bars)
{

	for (unsigned int i = 0; i < maxBars*4; i+=4)
	{
		if (i/4 <= bars)
		{
			hpBarImage[i] = (char)hpColor.x;
			hpBarImage[i + 1] = (char)hpColor.y;
			hpBarImage[i + 2] = (char)hpColor.z;
			hpBarImage[i + 3] = 255;
		}
		else
		{
			hpBarImage[i] = 0;
			hpBarImage[i + 1] = 0;
			hpBarImage[i + 2] = 0;
			hpBarImage[i + 3] = 0;
		}
	}
}

void HPBar::input()
{
	if (Input::keyStates['x'])
		player->healHp(1);
}