#include "HPBar.h"
#include "Texture.h"
#include "Player.h"
#include "Entity.h"
#include "Primitive.h"
#include "Input.h"

#include <iostream>


HPBar::HPBar(Player* player)
{
	this->player = player;
	this->maxBars = NUMBARS;
	this->numBars = NUMBARS;									// Start with full hp
	this->prevBars = 0;

	hpBar = new DynamicTexture(maxBars, 1, false, MIPMAPBIAS);	// Create Texture
	hpBarImage = new unsigned char[4 * maxBars];				// Allocate memory for image
	hpColor = highHP;
	setHP(maxBars);
	hpBar->setLoadedImage(hpBarImage);							// Set loaded image address
	hpBar->genDynamicGLTexture();

	maxHP = player->getTotalMaximumHp();

	mesh = new Mesh(new Quad(glm::vec3(0, -0.5, 0), 0.5f, 0.1f));
	Transform* transform = new Transform(
		glm::vec3(0, 0, 0), 30, glm::vec3(1, 0, 0), glm::vec3(3,1.5f,3)
		);
	quad = new Entity(transform, mesh, hpBar);
}


HPBar::~HPBar()
{
	if (hpBarImage)
		delete[] hpBarImage;
	if (quad)
		delete quad;
	if (mesh)
		delete mesh;
	if (hpBar)
		delete hpBar;
}

void HPBar::update()
{
	glm::vec3 playerPos = player->getTransform()->getPosition();
	quad->getTransform()->translate(playerPos.x, playerPos.y-1.7f, playerPos.z+1);

	unsigned int currentHP = player->getHp();
	float percentHP = (float)currentHP / (float)player->getTotalMaximumHp();
	numBars = (unsigned int)(percentHP * maxBars);

	if (percentHP > 0.8f)
		hpColor = highHP;
	else if (percentHP <= 0.8f && percentHP >= 0.2f)
		hpColor = mediumHP;
	else if (percentHP < 0.2f)
		hpColor = lowHP;

	if (prevBars != numBars)
	{
		prevBars = numBars;
		setHP(numBars);
		glBindTexture(GL_TEXTURE_2D, hpBar->textureID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, maxBars, 1, GL_RGBA, GL_UNSIGNED_BYTE, hpBarImage);
	}
}

void HPBar::setHP(unsigned int bars)
{

	for (unsigned int i = 0; i < maxBars*4; i+=4)
	{
		if (i/4 < bars)
		{
			hpBarImage[i] = (char)hpColor.r;
			hpBarImage[i + 1] = (char)hpColor.g;
			hpBarImage[i + 2] = (char)hpColor.b;
			hpBarImage[i + 3] = 255;
		}
		else
		{
			hpBarImage[i] = (char)backgroundHP.r;
			hpBarImage[i + 1] = (char)backgroundHP.g;
			hpBarImage[i + 2] = (char)backgroundHP.b;
			hpBarImage[i + 3] = BACKGROUND;
		}
	}
}

void HPBar::input()
{
	
}


void HPBar::print()
{
	for (unsigned int i = 0; i < 4 * maxBars; i++)
	{
		std::cout << (int)hpBarImage[i] << " ";
		if ((i+1) % 4 == 0)
			std::cout << i/4 <<": "<< std::endl;
	}
}

