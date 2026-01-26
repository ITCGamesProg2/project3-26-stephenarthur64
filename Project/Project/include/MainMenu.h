#pragma once
#include "raylib.h"

class MainMenu
{
public:
	MainMenu();
	void update();
	void draw();
	bool ended() { return m_end; }

private:
	bool m_end;
};

