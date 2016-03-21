#include "Audio.h"
#include <SFML\Audio.hpp>

std::vector<Audio*> Audio::sounds = std::vector<Audio*>();
std::vector<Audio*> Audio::musics = std::vector<Audio*>();
unsigned int Audio::musicVolume = 100;
unsigned int Audio::soundVolume = 100;

Audio::Audio(std::string path, AudioType type)
{
	this->type = type;
	this->path = path;

	if (type == AudioType::SOUND)
	{
		if (!this->buffer.loadFromFile(path))
			throw ("Sound Error. Path: " + path);

		this->sound.setBuffer(buffer);
		this->setVolume(Audio::soundVolume);
		Audio::sounds.push_back(this);
	}
	else if (type == AudioType::MUSIC)
	{
		if (!music.openFromFile(path))
			throw ("Music Error. Path: " + path);
		this->setVolume(Audio::musicVolume);
		Audio::musics.push_back(this);
	}
}

Audio::~Audio()
{
	if (this->type == AudioType::MUSIC)
	{
		for (int i = 0; i < Audio::musics.size(); i++)
			if (Audio::musics[i] == this)
				Audio::musics.erase(Audio::musics.begin() + i);
	}
	else if (this->type == AudioType::SOUND)
	{
		for (int i = 0; i < Audio::sounds.size(); i++)
			if (Audio::sounds[i] == this)
				Audio::sounds.erase(Audio::sounds.begin() + i);
	}
}

void Audio::setMusicVolume(unsigned int volume)
{
	for (Audio* audio : Audio::musics)
		audio->setVolume(volume);
	Audio::musicVolume = volume;
}

unsigned int Audio::getMusicVolume()
{
	return Audio::musicVolume;
}

void Audio::setSoundVolume(unsigned int volume)
{
	for (Audio* audio : Audio::sounds)
		audio->setVolume(volume);
	Audio::soundVolume = volume;
}

unsigned int Audio::getSoundVolume()
{
	return Audio::soundVolume;
}

void Audio::setVolume(unsigned int volume)
{
	if (this->type == AudioType::SOUND)
		this->sound.setVolume(volume);
	else if (this->type == AudioType::MUSIC)
		this->music.setVolume(volume);
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

AudioType Audio::getType()
{
	return this->type;
}

std::string Audio::getPath()
{
	return this->path;
}