#include "MainMenu.h"

MainMenu::MainMenu() : m_end(false)
{
}

void MainMenu::update()
{
	if (IsKeyReleased(KEY_SPACE))
	{
		m_end = true;
	}
}

void MainMenu::draw()
{
	DrawText("Main Menu", 100, 100, 50, WHITE);
}