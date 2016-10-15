#include "Audio.h"
#include <SFML\Audio.hpp>

Audio::Audio(std::string path, AudioType type, unsigned int volume)
{
	this->type = type;
	this->path = path;
	
	if (type == AudioType::SOUND)
	{
		if (!this->buffer.loadFromFile(path))
			throw ("Sound Error. Path: " + path);

		this->sound.setBuffer(buffer);
		this->setVolume(volume);
	}
	else if (type == AudioType::MUSIC)
	{
		if (!music.openFromFile(path))
			throw ("Music Error. Path: " + path);
		this->setVolume(volume);
	}
	
}

Audio::Audio(std::string path, AudioType type) : Audio(path, type, AUDIO_DEFAULT_VOLUME)
{
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

void Audio::setLoop(bool remainsOnLoop)
{
	if (this->type == AudioType::SOUND)
		this->sound.setLoop(remainsOnLoop);
	else if (this->type == AudioType::MUSIC)
		this->music.setLoop(remainsOnLoop);
}

void Audio::setVolume(unsigned int volume)
{
	if (this->type == AudioType::SOUND)
		this->sound.setVolume((float)volume);
	else if (this->type == AudioType::MUSIC)
		this->music.setVolume((float)volume);
}

AudioType Audio::getType()
{
	return this->type;
}

std::string Audio::getPath()
{
	return this->path;
}