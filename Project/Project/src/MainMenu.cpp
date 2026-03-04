#include "MainMenu.h"

MainMenu::MainMenu() : m_end(false)
{
}

void MainMenu::update()
{
	if (!IsMusicStreamPlaying(AssetManager::getMusic("title")))
	{
		PlayMusicStream(AssetManager::getMusic("title"));
	}
	else
	{
		UpdateMusicStream(AssetManager::getMusic("title"));
	}

	if (IsKeyReleased(KEY_SPACE))
	{
		m_end = true;
		LevelLoader::clearFile();
		StopMusicStream(AssetManager::getMusic("title"));
	}
	if (IsKeyReleased(KEY_S))
	{
		m_end = true;
		LevelLoader::loadFile();
		StopMusicStream(AssetManager::getMusic("title"));
	}
}

void MainMenu::draw()
{
	DrawText("Main Menu", 100, 100, 50, WHITE);
}

void MainMenu::resetMenu()
{
	PlayMusicStream(AssetManager::getMusic("title"));
	m_end = false;
}
