#pragma once
#include <raylib.h>
#include <map>
#include <string>

static class AssetManager
{
public:
	static void initSprites();
	static Texture2D& getSprite(std::string t_entity);
};

