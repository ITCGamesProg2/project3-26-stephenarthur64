#include "Button.h"

Button::Button() : m_size({0, 0}), m_text(""), m_position({0, 0}), m_trigger(false)
{
}

void Button::draw()
{
	if (m_fullSelect)
	{
		DrawRectangle(m_position.x - 5, m_position.y - 5, m_size.x + 10, m_size.y + 10, GREEN);
	}

	if (m_hover)
	{
		DrawRectangle(m_position.x - 5, m_position.y - 5, m_size.x + 10, m_size.y + 10, WHITE);
		DrawRectangle(m_position.x, m_position.y, m_size.x, m_size.y, DARKGRAY);
	}
	else
	{
		DrawRectangle(m_position.x, m_position.y, m_size.x, m_size.y, GRAY);
	}
	DrawText(m_text.c_str(), m_position.x + (10 * (m_text.size() / 4.0f)), m_position.y + 10, 20, WHITE);
}

void Button::detectClick(Vector2 t_mouse)
{
	if (t_mouse.x < m_position.x + m_size.x && t_mouse.x > m_position.x &&
		t_mouse.y < m_position.y + m_size.y && t_mouse.y > m_position.y)
	{
		m_trigger = true;
		m_hover = false;

		if (m_clickSound == nullptr)
		{
			m_clickSound = &AssetManager::getSound("confirm");
		}
		PlaySound(*m_clickSound);
	}
}

void Button::detectHover(Vector2 t_mouse)
{
	if (m_trigger)
	{
		return;
	}

	if (t_mouse.x < m_position.x + m_size.x && t_mouse.x > m_position.x &&
		t_mouse.y < m_position.y + m_size.y && t_mouse.y > m_position.y)
	{
		if (!m_hover)
		{
			PlaySound(AssetManager::getSound("click"));
		}
		m_hover = true;
	}
	else
	{
		m_hover = false;
	}
}

void Button::setPosition(Vector2 t_pos)
{
	m_position = { t_pos.x - (m_size.x / 2.0f), t_pos.y - (m_size.y / 2.0f) };
}

