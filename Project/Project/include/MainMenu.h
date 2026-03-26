#pragma once
#include "raylib.h"
#include "LevelLoader.h"
#include "AssetManager.h"
#include "Button.h"
#include "Exit.h"
#include "globals.h"
#include <string>

enum class MenuState {
	TITLE,
	SAVES,
	OPTIONS,
	END
};

enum MainButtons {
	PLAY,
	OPTIONS,
	QUIT
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
	void creditsUpdate();

	void draw();
	bool ended() { return m_end; }
	void resetMenu();
	void startGame(int t_file);
	void endGame() { m_state = MenuState::END; }
	void forceOptions() { m_state = MenuState::OPTIONS; m_forcedOptions = true; }

private:
	bool m_end;
	bool m_forcedOptions;
	Button m_main[3];
	Button m_saves[3];
	Button m_back;
	Button m_musicSlider;
	Button m_sfxSlider;
	MenuState m_state;
	Music* m_music;
	std::string m_title;
	std::string m_filesText;
	SaveDetails m_tempSave;
	Texture2D* m_powersSprite;
	Texture2D* m_backgroundSprite;
	Vector2 m_saveDetailsPos;
	Vector2 m_bufferMouse;
	float m_newMusicVolume;
	float m_newSFXVolume;
};

