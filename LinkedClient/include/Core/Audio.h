#include <string>
#include <SFML\Audio.hpp>

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
	~Audio();
	void play();
	void pause();
	void rewind();
	void setLoop(bool remainsOnLoop);
	void setVolume(unsigned int volume);
	unsigned int getVolume();
	AudioType getType();
	std::string getPath();
private:
	sf::Sound sound;
	sf::Music music;
	sf::SoundBuffer buffer;
	AudioType type;
	std::string path;
};

