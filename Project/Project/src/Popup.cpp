#include "Popup.h"

Popup::Popup() : m_currentSize({0, 0}), m_increaseSpeed(10.0f), m_alive(false), m_fontSize(20), m_maxed(false)
{
	m_tutorials["movement"] = "W/A/S/D or Up/Down/Left/Right to move";
	m_tutorials["attack"] = "LMB to Light Attack\nRMB to Heavy Attack\nLight attack is faster but does less damage\nHeavy attack is slower but does more damage";
	m_tutorials["health and momentum"] = "The red bar is health. When it runs out, you die\nThe gray bar is Momentum. This powers your\nTime Abilities and increases by attacking enemies";
	m_tutorials["rewind"] = "You unlocked the power to Rewind!\nPress and hold Q to rewind your timeline\nThis restores your health and position but not your\nMomentum.";
	m_tutorials["skip"] = "You unlocked the power to Skip!\nPress R to skip forward in time, avoiding any attacks\nor enemies in your way!";
	m_tutorials["stop"] = "You unlocked the power to Stop! The ultimate control.\nPress SPACE to stop time, freezing everything but\nyourself in time.\nThis ability can only be used at max Momentum";
}

void Popup::spawn(Vector2 t_size, Vector2 t_pos)
{
	m_alive = true;

	m_position = t_pos;
	m_size = t_size;

	m_closeButton.setSize({ 100.0f, 50.0f });
	m_closeButton.setPosition({ m_position.x + (m_size.x / 2.0f), m_position.y + (m_size.y - 30.0f) });
	m_closeButton.setText("Close");
}

void Popup::update()
{
	if (m_alive)
	{
		if (m_currentSize.x < m_size.x)
		{
			m_currentSize.x += m_increaseSpeed;
		}
		if (m_currentSize.y < m_size.y)
		{
			m_currentSize.y += m_increaseSpeed;
		}

		if (m_currentSize.x >= m_size.x && m_currentSize.y >= m_size.y)
		{
			m_maxed = true;
		}

		if (m_maxed)
		{
			m_closeButton.detectHover(GetMousePosition());

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
			{
				m_closeButton.detectClick(GetMousePosition());
			}
		}
	}
}

void Popup::draw()
{
	if (m_alive)
	{
		DrawRectangle(m_position.x, m_position.y, m_currentSize.x, m_currentSize.y, SKYBLUE);

		if (m_maxed)
		{
			DrawText(m_text.c_str(), m_position.x + m_fontSize, m_position.y + m_fontSize, m_fontSize, BLACK);
			m_closeButton.draw();
		}
	}
}

void Popup::setTutorial(std::string t_key)
{
	if (m_tutorials.count(t_key) > 0)
	{
		m_text = m_tutorials[t_key];
	}
	else
	{
		m_text = "No tutorial for " + t_key + " found";
	}
}
