#include "GameEntity.h"

GameEntity::GameEntity(Transform* transform, Mesh* mesh, Texture* texture) : Entity(transform, mesh, texture)
{
}

GameEntity::~GameEntity()
{
}

std::string GameEntity::getName()
{
	return this->name;
}

void GameEntity::setName(std::string name)
{
	this->name = name;
}

unsigned int GameEntity::getSize()
{
	return this->size;
}

void GameEntity::setSize(unsigned int size)
{
	this->size = size;
}

glm::vec3 GameEntity::getMapColor()
{
	return mapColor;
}

void GameEntity::setMapColor(glm::vec3 mapColor)
{
	this->mapColor = mapColor;
}

int GameEntity::getMapColorRed()
{
	return (int)this->mapColor.r;
}

void GameEntity::setMapColorRed(int red)
{
	this->mapColor.r = (float)red;
}

int GameEntity::getMapColorGreen()
{
	return (int)this->mapColor.g;
}

void GameEntity::setMapColorGreen(int green)
{
	this->mapColor.g = (float)green;
}

int GameEntity::getMapColorBlue()
{
	return (int)this->mapColor.b;
}

void GameEntity::setMapColorBlue(int blue)
{
	this->mapColor.b = (float)blue;
}