#include "AssetManager.h"
static std::map<std::string, Texture2D> m_textures;

void AssetManager::initSprites()
{
	m_textures["player"] = LoadTexture("img/GameObjects/player.png");
	m_textures["wall"] = LoadTexture("img/Environment/wall.png");
	m_textures["door"] = LoadTexture("img/Environment/door.png");
	m_textures["light"] = LoadTexture("img/GameObjects/lightenemy.png");
	m_textures["heavy"] = LoadTexture("img/GameObjects/heavyenemy.png");
	m_textures["support"] = LoadTexture("img/GameObjects/supportenemy.png");
	m_textures["powers"] = LoadTexture("img/UI/time manip icons.png");
}

Texture2D& AssetManager::getSprite(std::string t_entity)
{
	if (m_textures.count(t_entity))
	{
		return m_textures[t_entity];
	}
}
