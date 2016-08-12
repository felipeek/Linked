#include "AudioController.h"

Audio AudioController::themeAudio(THEME_AUDIO_PATH, AudioType::MUSIC, AUDIO_DEFAULT_VOLUME);
Audio AudioController::playerJoinedAudio(PLAYER_JOINED_AUDIO_PATH, AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::playerDisconnectedAudio(PLAYER_DISCONNECTED_AUDIO_PATH, AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::hoshoyoExplosionSkillAudio(HOSHOYO_EXPLOSION_SKILL_AUDIO_PATH, AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::playerTookDamageAudio(PLAYER_TOOK_DAMAGE_AUDIO_PATH, AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::playerAttackAudio(PLAYER_ATTACK_AUDIO_PATH, AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::turretAttackAudio(TURRET_ATTACK_AUDIO_PATH, AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::monsterTookDamageAudio(MONSTER_TOOK_DAMAGE_AUDIO_PATH, AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::monsterDiedAudio(MONSTER_DIED_AUDIO_PATH, AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
unsigned int AudioController::musicVolume = AUDIO_DEFAULT_VOLUME;
unsigned int AudioController::soundVolume = AUDIO_DEFAULT_VOLUME;

void AudioController::setMusicVolume(unsigned int volume)
{
	AudioController::themeAudio.setVolume(volume);
	AudioController::musicVolume = volume;
}

unsigned int AudioController::getMusicVolume()
{
	return AudioController::musicVolume;
}

void AudioController::setSoundVolume(unsigned int volume)
{
	// REMEMBER TO REFRESH WHEN ADDING NEW SOUNDS !
	AudioController::playerJoinedAudio.setVolume(volume);
	AudioController::playerDisconnectedAudio.setVolume(volume);
	AudioController::hoshoyoExplosionSkillAudio.setVolume(volume);
	AudioController::playerTookDamageAudio.setVolume(volume);
	AudioController::playerAttackAudio.setVolume(volume);
	AudioController::turretAttackAudio.setVolume(volume);
	AudioController::monsterTookDamageAudio.setVolume(volume);
	AudioController::monsterDiedAudio.setVolume(volume);
	AudioController::soundVolume = volume;
}

unsigned int AudioController::getSoundVolume()
{
	return AudioController::soundVolume;
}

/* GAME AUDIO */

Audio& AudioController::getThemeAudio()
{
	return AudioController::themeAudio;
}

Audio& AudioController::getPlayerJoinedAudio()
{
	return AudioController::playerJoinedAudio;
}

Audio& AudioController::getPlayerDisconnectedAudio()
{
	return AudioController::playerDisconnectedAudio;
}

Audio& AudioController::getHoshoyoExplosionSkillAudio()
{
	return AudioController::hoshoyoExplosionSkillAudio;
}

Audio& AudioController::getPlayerTookDamageAudio()
{
	return AudioController::playerTookDamageAudio;
}

Audio& AudioController::getPlayerAttackAudio()
{
	return AudioController::playerAttackAudio;
}

Audio& AudioController::getTurretAttackAudio()
{
	return AudioController::turretAttackAudio;
}

Audio& AudioController::getMonsterTookDamageAudio()
{
	return AudioController::monsterTookDamageAudio;
}

Audio& AudioController::getMonsterDiedAudio()
{
	return AudioController::monsterDiedAudio;
}