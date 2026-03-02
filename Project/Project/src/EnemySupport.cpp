#include "EnemySupport.h"

void EnemySupport::update()
{
	m_attack->setStart(m_position);
	move();
	GameObject::update();
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
			setTarget(e.getPosition());
			addForce(0.2f);
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
			setTarget(e.getPosition());
			addForce(0.2f);
			m_nearestDist = m_tempDist;
		}
	}
}
