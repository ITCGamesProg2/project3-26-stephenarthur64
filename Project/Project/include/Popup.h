#pragma once
#include "raylib.h"
#include <raymath.h>
#include <string>
#include <map>

#include "Button.h"

class Popup
{
public:
	Popup();
	void spawn(Vector2 t_size, Vector2 t_pos);
	void update();
	void draw();
	void setTutorial(std::string t_key);
	bool closeTriggered() { return m_closeButton.triggered(); }
	bool isAlive() { return m_alive; }
	void disable() 
	{ 
		m_alive = false; 
		m_closeButton.resetTrigger(); 
	}

private:
	Vector2 m_position;
	Vector2 m_size;
	Vector2 m_currentSize;
	float m_increaseSpeed;
	std::string m_text;
	Button m_closeButton;
	bool m_alive;
	int m_fontSize;
	bool m_maxed;

	std::map<std::string, std::string> m_tutorials;
};

