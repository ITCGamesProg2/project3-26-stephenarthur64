#include "MainMenu.h"

MainMenu::MainMenu() : m_end(false), m_state(MenuState::TITLE)
{
	init();
}

void MainMenu::init()
{
	m_playGame.setPosition({ 200.0f, 300.0f });
	m_playGame.setSize({ 100.0f, 50.0f });
	m_playGame.setText("Play");

	for (int i = 0; i < 3; i++)
	{
		m_saves[i].setPosition({ 200.0f + (120 * i), 500.0f });
		m_saves[i].setSize({ 100.0f, 50.0f });
		m_saves[i].setText("Save " + std::to_string(i + 1));
	}
}

void MainMenu::update()
{
	if (m_music == nullptr)
	{
		m_music = &AssetManager::getMusic("title");
	}
	if (!IsMusicStreamPlaying(*m_music))
	{
		PlayMusicStream(*m_music);
	}
	else
	{
		UpdateMusicStream(*m_music);
	}

	switch (m_state)
	{
	case MenuState::TITLE:
		titleUpdate();
		break;
	case MenuState::SAVES:
		savesUpdate();
		break;
	case MenuState::SETTINGS:
		settingsUpdate();
		break;
	default:
		break;
	}
}

void MainMenu::titleUpdate()
{
	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		m_playGame.detectClick(GetMousePosition());
	}

	if (m_playGame.triggered())
	{
		m_state = MenuState::SAVES;
	}
}

void MainMenu::savesUpdate()
{
	for (int i = 0; i < 3; i++)
	{
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
			m_saves[i].detectClick(GetMousePosition());
		}

		if (m_saves[i].triggered())
		{
			startGame(i + 1);
		}
	}
}

void MainMenu::settingsUpdate()
{
}

void MainMenu::draw()
{
	if (m_state == MenuState::TITLE)
	{
		DrawText("Main Menu", 100, 100, 50, WHITE);
		m_playGame.draw();
	}
	else if (m_state == MenuState::SAVES)
	{
		DrawText("Select a save file", 100, 100, 50, WHITE);

		for (int i = 0; i < 3; i++)
		{
			m_saves[i].draw();
		}
	}
}

void MainMenu::resetMenu()
{
	PlayMusicStream(AssetManager::getMusic("title"));
	m_end = false;
}

void MainMenu::startGame(int t_file)
{
	m_end = true;
	LevelLoader::loadFile(t_file);
	StopMusicStream(AssetManager::getMusic("title"));
}
