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
	static void setMusicVolume(unsigned int volume);
	static unsigned int getMusicVolume();
	static void setSoundVolume(unsigned int volume);
	static unsigned int getSoundVolume();
	void play();
	void pause();
	void rewind();
	void setLoop(bool remainsOnLoop);
	AudioType getType();
	std::string getPath();
private:
	static std::vector<Audio*> sounds;
	static std::vector<Audio*> musics;
	static unsigned int musicVolume;
	static unsigned int soundVolume;

	void setVolume(unsigned int volume);
	sf::Sound sound;
	sf::Music music;
	sf::SoundBuffer buffer;
	AudioType type;
	std::string path;
};

