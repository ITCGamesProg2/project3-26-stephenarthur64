#include "LightAttack.h"

LightAttack::LightAttack(Vector2 t_start)
{
	m_startPos = t_start;
	m_width = 100.0f;
	m_height = 20.0f;
	m_speed = 5.0f;
	m_maxDifference = 60.0f;
	m_minAngle = 0.0f;
	m_maxAngle = m_minAngle + m_maxDifference;
}

void LightAttack::execute()
{
	if (!m_running)
	{
		m_running = true;
		m_currentAngle = m_minAngle;
	}
}

void LightAttack::process()
{
	if (m_running)
	{
		m_currentAngle += m_speed;

		if (m_currentAngle > m_maxAngle)
		{
			m_running = false;
		}
	}
}

void LightAttack::draw()
{
	DrawRectanglePro({ m_startPos.x, m_startPos.y,  m_width, m_height }, { 0.0f, m_height / 2 }, m_currentAngle, YELLOW);
}
