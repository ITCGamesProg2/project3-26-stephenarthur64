#include "SupportAttack.h"

SupportAttack::SupportAttack(Vector2 t_start)
{
	m_running = false;
	m_startPos = t_start;
	m_width = 10.0f;
	m_height = 10.0f;
	m_type = SUPPORT;
}

void SupportAttack::execute(Vector2 t_target)
{
	m_running = true;
	m_cooldown = 0.0f;
	m_windup = 0.0f;
	m_duration = 0.0f;
	m_targetPos = t_target;
}

void SupportAttack::process()
{
}

void SupportAttack::draw()
{
	if (m_running)
	{
		DrawLine(m_startPos.x, m_startPos.y, m_targetPos.x, m_targetPos.y, SKYBLUE);
	}
}
