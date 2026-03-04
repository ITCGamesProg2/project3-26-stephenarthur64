#pragma once
#include <raylib.h>
#include <map>
#include <string>

static class AssetManager
{
public:
	static void initAssets();
	static void setVolume(float t_volume);
	static Texture2D& getSprite(std::string t_entity);
	static Sound& getSound(std::string t_sound);
	static Music& getMusic(std::string t_music);
};

