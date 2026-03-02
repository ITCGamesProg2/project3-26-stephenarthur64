#pragma once
#include <raylib.h>

static class AssetManager
{
public:
	static void initSprites();
	static Texture2D& getPlayerSprite();
	static Texture2D& getWallSprite();
	static Texture2D& getDoorSprite();
	static Texture2D& getLightSprite();
	static Texture2D& getHeavySprite();
	static Texture2D& getSupportSprite();
};

