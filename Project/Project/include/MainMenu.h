#pragma once
#include "raylib.h"

class MainMenu
{
public:
	MainMenu();
	void update();
	void draw();
	bool ended() { return m_end; }
	void resetMenu();

private:
	bool m_end;
};

