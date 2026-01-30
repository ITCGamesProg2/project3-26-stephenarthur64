#include "Attack.h"
#include <iostream>

void Attack::execute(Vector2 t_target)
{
	if (!m_running && m_cooldown >= m_maxCooldown)
	{
		m_running = true;
		m_cooldown = 0.0f;
		m_windup = 0.0f;
		m_duration = 0.0f;
		m_targetPos = t_target - m_startPos;
		m_targetPos = Vector2Normalize(m_targetPos);
		m_targetPos *= m_hitboxOffset;
	}
}

void Attack::process()
{
	if (m_running)
	{
		m_windup += GetFrameTime();

		if (m_windup >= m_maxWindup)
		{
			m_duration += GetFrameTime();

			if (m_duration < m_maxDuration)
			{
				m_startPos.x += m_targetPos.x;
				m_startPos.y += m_targetPos.y;
			}
			else
			{
				m_running = false;
				m_windup = 0.0f;
			}
		}
	}
	else
	{
		m_cooldown += GetFrameTime();
	}
}

Rectangle Attack::getHitbox()
{
	if (m_running)
	{
		return { m_startPos.x - m_width / 2.0f, m_startPos.y - m_height / 2.0f, m_width, m_height };
	}
	else
	{
		return { -1000, -1000, 0, 0 };
	}
}

void Attack::setStart(Vector2 t_start)
{
	m_startPos = t_start; 
}

bool Attack::canAttack()
{
	if (m_running || m_cooldown <= m_maxCooldown)
	{
		return false;
	}
	return true;
}

bool Attack::isCollided()
{
	if (m_collided)
	{
		m_collided = false;
		return true;
	}
	return false;
}

void Attack::draw()
{
	if (m_running)
	{
		DrawRectanglePro({ m_startPos.x, m_startPos.y,  m_width, m_height }, { m_width / 2.0f, m_height / 2.0f }, 0.0f, YELLOW);
	}
}
