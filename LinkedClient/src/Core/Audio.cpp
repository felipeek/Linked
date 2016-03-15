#include "Audio.h"
#include <SFML\Audio.hpp>

Audio::Audio(std::string path, AudioType type)
{
	this->type = type;
	this->path = path;

	if (type == AudioType::SOUND)
	{
		if (!this->buffer.loadFromFile(path))
			throw ("Sound Error. Path: " + path);

		this->sound.setBuffer(buffer);
	}
	else if (type == AudioType::MUSIC)
	{
		if (!music.openFromFile(path))
			throw ("Music Error. Path: " + path);
	}
}

Audio::~Audio()
{
}

void Audio::play()
{
	if (this->type == AudioType::SOUND)
		this->sound.play();
	else if (this->type == AudioType::MUSIC)
		this->music.play();
}

void Audio::pause()
{
	if (this->type == AudioType::SOUND)
		this->sound.pause();
	else if (this->type == AudioType::MUSIC)
		this->music.pause();
}

void Audio::rewind()
{
	if (this->type == AudioType::SOUND)
		this->sound.stop();
	else if (this->type == AudioType::MUSIC)
		this->music.stop();
}

void Audio::setVolume(unsigned int volume)
{
	if (this->type == AudioType::SOUND)
		this->sound.setVolume(volume);
	else if (this->type == AudioType::MUSIC)
		this->music.setVolume(volume);
}

void Audio::setLoop(bool remainsOnLoop)
{
	if (this->type == AudioType::SOUND)
		this->sound.setLoop(remainsOnLoop);
	else if (this->type == AudioType::MUSIC)
		this->music.setLoop(remainsOnLoop);
}

unsigned int Audio::getVolume()
{
	if (this->type == AudioType::SOUND)
		return this->sound.getVolume();
	else if (this->type == AudioType::MUSIC)
		return this->music.getVolume();
}

AudioType Audio::getType()
{
	return this->type;
}

std::string Audio::getPath()
{
	return this->path;
}