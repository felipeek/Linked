#include <string>
#include <SFML\Audio.hpp>

#define AUDIO_DEFAULT_VOLUME 100

enum class AudioType
{
	SOUND,
	MUSIC
};

#pragma once
class Audio
{
public:
	Audio(std::string path, AudioType type);
	Audio(std::string path, AudioType type, unsigned int volume);
	~Audio();
	void play();
	void pause();
	void rewind();
	void setLoop(bool remainsOnLoop);
	void setVolume(unsigned int volume);
	AudioType getType();
	std::string getPath();

private:
	sf::Sound sound;
	sf::Music music;
	sf::SoundBuffer buffer;
	AudioType type;
	std::string path;
};

