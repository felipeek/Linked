#pragma once
#include "Audio.h"

#define THEME_AUDIO_PATH "./res/Audio/theme.wav"
#define PLAYER_JOINED_AUDIO_PATH "./res/Audio/player_joined.wav"
#define PLAYER_DISCONNECTED_AUDIO_PATH "./res/Audio/player_disconnected.wav"
#define HOSHOYO_EXPLOSION_SKILL_AUDIO_PATH "./res/Audio/hoshoyo_explosion_skill.wav"
#define PLAYER_TOOK_DAMAGE_AUDIO_PATH "./res/Audio/player_hit.wav"
#define PLAYER_ATTACK_AUDIO_PATH "./res/Audio/attack.wav"
#define TURRET_ATTACK_AUDIO_PATH "./res/Audio/enemy_attack.wav"
#define MONSTER_TOOK_DAMAGE_AUDIO_PATH "./res/Audio/monster_hit.wav"
#define MONSTER_DIED_AUDIO_PATH "./res/Audio/monster_dead.wav"


class AudioController
{
public:
	static void setMusicVolume(unsigned int volume);
	static unsigned int getMusicVolume();
	static void setSoundVolume(unsigned int volume);
	static unsigned int getSoundVolume();
	
	/* GAME AUDIO */
	static Audio& getThemeAudio();
	static Audio& getPlayerJoinedAudio();
	static Audio& getPlayerDisconnectedAudio();
	static Audio& getHoshoyoExplosionSkillAudio();
	static Audio& getPlayerTookDamageAudio();
	static Audio& getPlayerAttackAudio();
	static Audio& getTurretAttackAudio();
	static Audio& getMonsterTookDamageAudio();
	static Audio& getMonsterDiedAudio();
private:
	static unsigned int musicVolume;
	static unsigned int soundVolume;
	static Audio themeAudio;
	static Audio playerJoinedAudio;
	static Audio playerDisconnectedAudio;
	static Audio hoshoyoExplosionSkillAudio;
	static Audio playerTookDamageAudio;
	static Audio playerAttackAudio;
	static Audio turretAttackAudio;
	static Audio monsterTookDamageAudio;
	static Audio monsterDiedAudio;
};