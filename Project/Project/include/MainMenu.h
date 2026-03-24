#pragma once
#include "raylib.h"
#include "LevelLoader.h"
#include "AssetManager.h"
#include "Button.h"

enum class MenuState {
	TITLE,
	SAVES,
	SETTINGS
};

class MainMenu
{
public:
	MainMenu();
	void init();

	void update();
	void titleUpdate();
	void savesUpdate();
	void settingsUpdate();

	void draw();
	bool ended() { return m_end; }
	void resetMenu();
	void startGame(int t_file);

private:
	bool m_end;
	Button m_playGame;
	Button m_saves[3];
	MenuState m_state;
	Music* m_music;
};

