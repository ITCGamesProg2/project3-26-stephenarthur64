#include "EnemySupport.h"

void EnemySupport::update()
{
	m_attack->setStart(m_position);

	if (!m_alive)
	{
		m_attack->setStart({ -1000.0f, 0.0f });
		m_position = { -1000.0f, 0.0f };
		return;
	}

	GameObject::update();

	if (Vector2DistanceSqr(m_target, m_position) < 200000)
	{
		if (Vector2DistanceSqr(m_target, m_position) > m_minDistance)
		{
			addForce(m_speed);
		}
		else
		{
			if (m_attack->canAttack())
			{
				m_attack->execute(m_target);
			}
		}

		m_attack->setStart(m_position);
		m_attack->process();

		move();
	}
}

void EnemySupport::findNearest(std::vector<NPC>& t_e)
{
	bool found = false;
	m_nearestDist = 200;

	for (NPC& e : t_e)
	{
		m_tempDist = Vector2Distance(e.getPosition(), m_position);
		if (m_tempDist < m_nearestDist)
		{
			found = true;
			m_attack->execute(e.getPosition()); 
			m_target = e.getPosition();
			m_nearestDist = m_tempDist;
		}
	}

	if (!found)
	{
		m_attack->stopAttack();
	}

	m_nearestDist = 500;

	for (NPC& e : t_e)
	{
		m_tempDist = Vector2Distance(e.getPosition(), m_position);
		if (m_tempDist < m_nearestDist)
		{
			m_target = e.getPosition();
			m_nearestDist = m_tempDist;
		}
	}
}
