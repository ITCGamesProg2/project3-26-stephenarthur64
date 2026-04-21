#include "AssetManager.h"
#include "Animation.h"
static std::map<std::string, Texture2D> m_textures;
static std::map<std::string, Sound> m_sounds;
static std::map<std::string, Music> m_music;
static std::map<std::string, Shader> m_shaders;

void AssetManager::initAssets()
{
	m_textures["player"] = LoadTexture("img/GameObjects/player.png");
	m_textures["wall"] = LoadTexture("img/Environment/wall.png");
	m_textures["door"] = LoadTexture("img/Environment/door.png");
	m_textures["light"] = LoadTexture("img/GameObjects/lightenemy.png");
	m_textures["heavy"] = LoadTexture("img/GameObjects/heavyenemy.png");
	m_textures["support"] = LoadTexture("img/GameObjects/supportenemy.png");
	m_textures["powers"] = LoadTexture("img/UI/time manip icons.png");
	m_textures["lightattack"] = LoadTexture("img/Attacks/lightattack.png");
	m_textures["heavyattack"] = LoadTexture("img/Attacks/heavyattack.png");
	m_textures["rewindboss"] = LoadTexture("img/GameObjects/RewindBoss.png");

	m_sounds["hurt"] = LoadSound("audio/sfx/Hurt.wav");
	m_sounds["swing"] = LoadSound("audio/sfx/Sword_Slash.wav");
	m_sounds["door_open"] = LoadSound("audio/sfx/Door_Slow_Open.wav");
	m_sounds["die"] = LoadSound("audio/sfx/Die.wav");
	m_sounds["bump"] = LoadSound("audio/sfx/Bump.wav");
	m_sounds["powerup"] = LoadSound("audio/sfx/Powerup.wav");
	m_sounds["timestop"] = LoadSound("audio/sfx/TimeStop.wav");
	m_sounds["timestopend"] = LoadSound("audio/sfx/TimeStopEnd.wav");
	m_sounds["click"] = LoadSound("audio/sfx/click.wav");
	m_sounds["confirm"] = LoadSound("audio/sfx/Confirm.wav");
	m_sounds["step"] = LoadSound("audio/sfx/8-bit-foot-steps.mp3");
	m_sounds["low_health"] = LoadSound("audio/sfx/Low_Health.wav");

	m_music["title"] = LoadMusicStream("audio/music/8_Bit_Nostalgia.mp3");
	m_music["main"] = LoadMusicStream("audio/music/8_Bit_Adventure.mp3");
	m_music["boss"] = LoadMusicStream("audio/music/Boss_time.mp3");

	m_shaders["stop"] = LoadShader(0, TextFormat("shaders/timestop.fs", GLSL_VERSION));
	m_shaders["rewind"] = LoadShader(0, TextFormat("shaders/rewind.fs", GLSL_VERSION));
	m_shaders["skip"] = LoadShader(0, TextFormat("shaders/skip.fs", GLSL_VERSION));
}

void AssetManager::setMusicVolume(float t_volume)
{
	std::map<std::string, Music>::iterator it;

	for (it = m_music.begin(); it != m_music.end(); it++)
	{
		SetMusicVolume(it->second, t_volume);
	}
}

void AssetManager::setSFXVolume(float t_volume)
{
	std::map<std::string, Sound>::iterator it;

	for (it = m_sounds.begin(); it != m_sounds.end(); it++)
	{
		SetSoundVolume(it->second, t_volume);
	}
}

Texture2D& AssetManager::getSprite(std::string t_entity)
{
	if (m_textures.count(t_entity))
	{
		return m_textures[t_entity];
	}
}

Sound& AssetManager::getSound(std::string t_sound)
{
	if (m_sounds.count(t_sound))
	{
		return m_sounds[t_sound];
	}
}

Music& AssetManager::getMusic(std::string t_music)
{
	if (m_music.count(t_music))
	{
		return m_music[t_music];
	}
}

Shader& AssetManager::getShader(std::string t_shader)
{
	if (m_shaders.count(t_shader))
	{
		return m_shaders[t_shader];
	}
}
