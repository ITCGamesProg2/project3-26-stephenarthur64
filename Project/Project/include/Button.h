#pragma once
#include <raylib.h>
#include <string>
#include <functional>

class Button
{
public:
	Button();
	void setSize(Vector2 t_size) { m_size = t_size; }
	void setPosition(Vector2 t_pos) { m_position = t_pos; }
	void setText(std::string t_text) { m_text = t_text; }
	void draw();
	void detectClick(Vector2 t_mouse);
	bool triggered() { return m_trigger; }

private:
	Vector2 m_size;
	Vector2 m_position;
	std::string m_text;
	bool m_trigger;
};

class MainMenu;

