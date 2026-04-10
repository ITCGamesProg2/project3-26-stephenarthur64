#include "MainMenu.h"

MainMenu::MainMenu() : m_end(false), m_state(MenuState::TITLE), m_title("Evanescent Gloom"), m_filesText("Select a save file"), m_bufferMouse({-1, -1}), m_forcedOptions(false),
						m_editSelected(false)
{
}

void MainMenu::init()
{
	for (int i = 0; i < 3; i++)
	{
		m_main[i].setSize({ 100.0f, 50.0f });
		m_main[i].setPosition({ SCREEN_WIDTH / 2.0f, (SCREEN_HEIGHT / 2.0f) + (75.0f * i) });
	}

	m_main[MainButtons::PLAY].setText("Play");

	m_main[MainButtons::OPTIONS].setText("Options");

	m_main[MainButtons::QUIT].setText("Quit");

	for (int i = 0; i < 3; i++)
	{
		m_saves[i].setSize({ 100.0f, 50.0f });
		m_saves[i].setPosition({ ((SCREEN_WIDTH / 2.0f) - 200) + (200 * i), 500.0f });
		m_saves[i].setText("Save " + std::to_string(i + 1));
	}

	m_edit.setSize({ 100.0f, 50.0f });
	m_edit.setPosition({ SCREEN_WIDTH - 200.0f, 500.0f });
	m_edit.setText("Edit");

	m_back.setSize({ 100.0f, 50.0f });
	m_back.setPosition({ 200, SCREEN_HEIGHT - 100 });
	m_back.setText("Back");

	m_musicSlider.setSize({ 50.0f, 50.0f });
	m_musicSlider.setPosition({ 500.0f, 300.0f });

	m_sfxSlider.setSize({ 50.0f, 50.0f });
	m_sfxSlider.setPosition({ 500.0f, 500.0f });

	LevelLoader::loadOptions();
	m_newMusicVolume = LevelLoader::getMusicVolume();
	m_newSFXVolume = LevelLoader::getSFXVolume();
	AssetManager::setMusicVolume(m_newMusicVolume);
	AssetManager::setSFXVolume(m_newSFXVolume);

	m_musicSlider.setPosition({ (m_newMusicVolume * (SCREEN_WIDTH - 200.0f)) + 100, 300.0f });
	m_sfxSlider.setPosition({ (m_newSFXVolume * (SCREEN_WIDTH - 200.0f)) + 100, 500.0f });
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

	m_back.setPosition({ 200, SCREEN_HEIGHT - 100 });

	switch (m_state)
	{
	case MenuState::TITLE:
		titleUpdate();
		break;
	case MenuState::SAVES:
		savesUpdate();
		break;
	case MenuState::OPTIONS:
		settingsUpdate();
		break;
	case MenuState::END:
		creditsUpdate();
	default:
		break;
	}
}

void MainMenu::titleUpdate()
{
	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		for (int i = 0; i < 3; i++)
		{
			m_main[i].detectClick(GetMousePosition());
		}
	}

	for (int i = 0; i < 3; i++)
	{
		m_main[i].detectHover(GetMousePosition());
	}

	if (m_main[MainButtons::PLAY].triggered())
	{
		m_state = MenuState::SAVES;
		m_main[MainButtons::PLAY].resetTrigger();
		LevelLoader::loadSaves();
	}
	else if (m_main[MainButtons::OPTIONS].triggered())
	{
		m_state = MenuState::OPTIONS;
		m_main[MainButtons::OPTIONS].resetTrigger();
	}
	else if (m_main[MainButtons::QUIT].triggered())
	{
		m_main[MainButtons::QUIT].resetTrigger();

		Exit::closeGame();
	}
}

void MainMenu::savesUpdate()
{
	for (int i = 0; i < 3; i++)
	{
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
			m_saves[i].detectClick(GetMousePosition());
			m_back.detectClick(GetMousePosition());
			m_edit.detectClick(GetMousePosition());
		}

		for (int i = 0; i < 3; i++)
		{
			m_saves[i].detectHover(GetMousePosition());
		}

		m_back.detectHover(GetMousePosition());
		m_edit.detectHover(GetMousePosition());

		if (m_saves[i].triggered())
		{
			startGame(i + 1);
			m_saves[i].resetTrigger();
		}

		if (m_back.triggered())
		{
			m_state = MenuState::TITLE;
			m_back.resetTrigger();
		}

		if (m_edit.triggered())
		{
			startGame(0);
			m_editSelected = true;
			m_edit.resetTrigger();
		}
	}
}

void MainMenu::settingsUpdate()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		m_musicSlider.detectClick(GetMousePosition());
		m_sfxSlider.detectClick(GetMousePosition());
	}
	else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		m_musicSlider.resetTrigger();
		m_sfxSlider.resetTrigger();
		m_back.detectClick(GetMousePosition());
	}

	m_musicSlider.detectHover(GetMousePosition());
	m_sfxSlider.detectHover(GetMousePosition());
	m_back.detectHover(GetMousePosition());

	if (m_musicSlider.triggered())
	{
		if (GetMousePosition().x > 100.0f && GetMousePosition().x < SCREEN_WIDTH - 150.0f)
		{
			m_musicSlider.newX(GetMousePosition().x);
			m_newMusicVolume = (GetMousePosition().x - 100.0f) / (SCREEN_WIDTH - 200.0f);
			AssetManager::setMusicVolume(m_newMusicVolume);
			LevelLoader::saveOptions(m_newMusicVolume, m_newSFXVolume);
		}
		m_musicSlider.forceHover();
	}

	if (m_sfxSlider.triggered())
	{
		if (GetMousePosition().x > 100.0f && GetMousePosition().x < SCREEN_WIDTH - 150.0f)
		{
			m_sfxSlider.newX(GetMousePosition().x);
			m_newSFXVolume = (GetMousePosition().x - 100.0f) / (SCREEN_WIDTH - 200.0f);
			AssetManager::setSFXVolume(m_newSFXVolume);
			LevelLoader::saveOptions(m_newMusicVolume, m_newSFXVolume);
		}
		m_sfxSlider.forceHover();
	}

	if (m_back.triggered())
	{
		if (m_forcedOptions)
		{
			m_forcedOptions = false;
			m_end = true;
			m_back.resetTrigger();
		}
		else
		{
			m_state = MenuState::TITLE;
			m_back.resetTrigger();
		}
	}
}

void MainMenu::creditsUpdate()
{
	m_back.setPosition({ (SCREEN_WIDTH / 2.0f) - 50, SCREEN_HEIGHT - 100 });

	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		m_back.detectClick(GetMousePosition());
	}

	m_back.detectHover(GetMousePosition());

	if (m_back.triggered())
	{
		m_state = MenuState::TITLE;
		m_back.resetTrigger();
	}
}

void MainMenu::draw()
{
	ClearBackground(WHITE);

	if (m_backgroundSprite == nullptr)
	{
		m_backgroundSprite = &AssetManager::getSprite("wall");
	}

	DrawTexturePro(*m_backgroundSprite, { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, { 0, 0 }, 0.0f, WHITE);

	if (m_state == MenuState::TITLE)
	{
		DrawText(m_title.c_str(), (SCREEN_WIDTH / 2.0f) - (50 * (m_title.size() / 4.0f)), 100, 50, WHITE);
		
		for (int i = 0; i < 3; i++)
		{
			m_main[i].draw();
		}
	}
	else if (m_state == MenuState::SAVES)
	{
		if (m_powersSprite == nullptr)
		{
			m_powersSprite = &AssetManager::getSprite("powers");
		}

		DrawText(m_filesText.c_str(), (SCREEN_WIDTH / 2.0f) - (50 * (m_filesText.size() / 4.0f)), 100, 50, WHITE);

		for (int i = 0; i < 3; i++)
		{
			m_saveDetailsPos.x = (SCREEN_WIDTH / 2.0f) - 250 + (200 * i);
			m_saveDetailsPos.y = 180;

			DrawRectangle(m_saveDetailsPos.x - 20, m_saveDetailsPos.y - 20, 150, 380, DARKGRAY);
			DrawRectangleLinesEx({ m_saveDetailsPos.x - 20, m_saveDetailsPos.y - 20, 150, 380 }, 10.0f, BLACK);

			m_saves[i].draw();

			m_tempSave = LevelLoader::getSaveDetails(i);

			DrawText(("Level: " + std::to_string(m_tempSave.level)).c_str(), m_saveDetailsPos.x, m_saveDetailsPos.y, 20, WHITE);
			DrawText(("Progress: " + std::to_string(m_tempSave.progress)).c_str(), m_saveDetailsPos.x, m_saveDetailsPos.y + 30, 20, WHITE);

			if (m_tempSave.rewind)
			{
				DrawTexturePro(*m_powersSprite, { 0, 0, 32, 32 }, { m_saveDetailsPos.x, m_saveDetailsPos.y + 60, 70, 70 }, { 0, 0 }, 0.0f, WHITE);
			}
			if (m_tempSave.skip)
			{
				DrawTexturePro(*m_powersSprite, { 32, 0, 32, 32 }, { m_saveDetailsPos.x, m_saveDetailsPos.y + 130, 70, 70 }, { 0, 0 }, 0.0f, WHITE);
			}
			if (m_tempSave.stop)
			{
				DrawTexturePro(*m_powersSprite, { 64, 0, 32, 32 }, { m_saveDetailsPos.x, m_saveDetailsPos.y + 200, 70, 70 }, { 0, 0 }, 0.0f, WHITE);
			}
		}

		m_back.draw();
		m_edit.draw();
	}
	else if (m_state == MenuState::OPTIONS)
	{
		DrawText("Music Volume", (SCREEN_WIDTH / 2.0f) - 100, 200.0f, 20, WHITE);
		DrawRectangle(100.0f, 300.0f, SCREEN_WIDTH - 200.0f, 10.0f, BLACK);
		m_musicSlider.draw();

		DrawText("SFX Volume", (SCREEN_WIDTH / 2.0f) - 100, 400.0f, 20, WHITE);
		DrawRectangle(100.0f, 500.0f, SCREEN_WIDTH - 200.0f, 10.0f, BLACK);
		m_sfxSlider.draw();

		m_back.draw();
	}
	else if (m_state == MenuState::END)
	{
		DrawText(m_title.c_str(), (SCREEN_WIDTH / 2.0f) - (30 * (m_title.size() / 2.0f)), 100, 50, WHITE);
		DrawText("A Game by Lucy Arthur", (SCREEN_WIDTH / 2.0f) - (30 * (m_title.size() / 2.0f)), 200, 30, WHITE);
		DrawText("Created with: Raylib", (SCREEN_WIDTH / 2.0f) - (30 * (m_title.size() / 2.0f)), 350, 30, WHITE);
		DrawText("Sound Effects: Coffee 'Valen' Bat", (SCREEN_WIDTH / 2.0f) - (30 * (m_title.size() / 2.0f)), 400, 30, WHITE);
		DrawText("Music: FesliyanStudios.com", (SCREEN_WIDTH / 2.0f) - (30 * (m_title.size() / 2.0f)), 450, 30, WHITE);

		m_back.draw();
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
	m_state = MenuState::TITLE;
}
