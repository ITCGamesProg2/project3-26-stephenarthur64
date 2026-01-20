#include "Attack.h"

void Attack::execute(Vector2 t_target)
{
	if (!m_running && m_cooldown >= m_maxCooldown)
	{
		m_running = true;
		m_cooldown = 0.0f;
		m_targetPos = t_target - m_startPos;
		m_targetPos = Vector2Normalize(m_targetPos);
		m_targetPos *= m_hitboxOffset;

		// Keeping here for when there is an asset for the attack
		/*m_targetAngle = -(atan2(m_startPos.x - t_target.x, m_startPos.y - t_target.y) * RAD2DEG) - 90;

		m_minAngle = m_targetAngle - (m_maxDifference / 2);
		m_maxAngle = m_targetAngle + (m_maxDifference / 2);

		if (m_direction == 1)
		{
			m_currentAngle = m_minAngle;
		}
		else
		{
			m_currentAngle = m_maxAngle;
		}*/
	}
}

void Attack::process()
{
	if (m_running)
	{
		m_currentAngle += m_speed * m_direction;
		m_startPos.x += m_targetPos.x;
		m_startPos.y += m_targetPos.y;

		if (m_currentAngle > m_maxAngle || m_currentAngle < m_minAngle)
		{
			m_running = false;
			m_direction = -m_direction;
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
