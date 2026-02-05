#include "MainMenu.h"

MainMenu::MainMenu() : m_end(false)
{
}

void MainMenu::update()
{
	if (IsKeyReleased(KEY_SPACE))
	{
		m_end = true;
		LevelLoader::clearFile();
	}
	if (IsKeyReleased(KEY_S))
	{
		m_end = true;
		LevelLoader::loadFile();
	}
}

void MainMenu::draw()
{
	DrawText("Main Menu", 100, 100, 50, WHITE);
}

void MainMenu::resetMenu()
{
	m_end = false;
}
