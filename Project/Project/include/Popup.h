#pragma once
#include "raylib.h"
#include <raymath.h>
#include <string>

#include "Button.h"

class Popup
{
public:
	Popup();
	void spawn(Vector2 t_size, Vector2 t_pos);
	void update();
	void draw();
	void setText(std::string t_text) { m_text = t_text; }

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
};

