#include "Button.h"

Button::Button() : m_size({0, 0}), m_text(""), m_position({0, 0}), m_trigger(false)
{
}

void Button::draw()
{
	DrawRectangle(m_position.x, m_position.y, m_size.x, m_size.y, GRAY);
	DrawText(m_text.c_str(), m_position.x, m_position.y, 20, WHITE);
}

void Button::detectClick(Vector2 t_mouse)
{
	if (t_mouse.x < m_position.x + m_size.x && t_mouse.x > m_position.x &&
		t_mouse.y < m_position.y + m_size.y && t_mouse.y > m_position.y)
	{
		m_trigger = true;
	}
}

