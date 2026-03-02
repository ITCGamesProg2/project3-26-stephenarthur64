#include "AssetManager.h"
static Texture2D m_playerSprite;
static Texture2D m_wallSprite;
static Texture2D m_doorSprite;
static Texture2D m_lightSprite;
static Texture2D m_heavySprite;
static Texture2D m_supportSprite;

void AssetManager::initSprites()
{
	m_playerSprite = LoadTexture("img/GameObjects/player.png");
	m_wallSprite = LoadTexture("img/Environment/wall.png");
	m_doorSprite = LoadTexture("img/Environment/door.png");
	m_lightSprite = LoadTexture("img/GameObjects/lightenemy.png");
	m_heavySprite = LoadTexture("img/GameObjects/heavyenemy.png");
	m_supportSprite = LoadTexture("img/GameObjects/supportenemy.png");
}

Texture2D& AssetManager::getPlayerSprite()
{
	return m_playerSprite;
}

Texture2D& AssetManager::getWallSprite()
{
	return m_wallSprite;
}

Texture2D& AssetManager::getDoorSprite()
{
	return m_doorSprite;
}

Texture2D& AssetManager::getLightSprite()
{
	return m_lightSprite;
}

Texture2D& AssetManager::getHeavySprite()
{
	return m_heavySprite;
}

Texture2D& AssetManager::getSupportSprite()
{
	return m_supportSprite;
}
