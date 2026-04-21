#pragma once
#include <raylib.h>
#include <string>
#include <functional>
#include "AssetManager.h"

class Button
{
public:
	Button();
	void setSize(Vector2 t_size) { m_size = t_size; }
	void setPosition(Vector2 t_pos);
	void setText(std::string t_text) { m_text = t_text; }
	void draw();
	void detectClick(Vector2 t_mouse);
	void detectHover(Vector2 t_mouse);
	bool triggered() { return m_trigger; }
	void resetTrigger() { m_trigger = false; }
	void newX(float t_x) { m_position.x = t_x; }
	void forceHover() { m_hover = true; }
	void fullSelect() { m_fullSelect = true; }
	void fullDeselect() { m_fullSelect = false; }
	bool isSelected() { return m_fullSelect; }

	Vector2 getPosition() { return m_position; }

private:
	Vector2 m_size;
	Vector2 m_position;
	std::string m_text;
	bool m_trigger;
	bool m_hover;
	Sound* m_clickSound;
	bool m_fullSelect;
};

class MainMenu;

