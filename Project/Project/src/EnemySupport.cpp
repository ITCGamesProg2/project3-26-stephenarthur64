#include "EnemySupport.h"

void EnemySupport::update()
{
	m_attack->setStart(m_position);
}

void EnemySupport::findNearest(std::vector<NPC>& t_e)
{
	for (NPC& e : t_e)
	{
		if (Vector2Distance(e.getPosition(), m_position) < 200)
		{
			m_attack->execute(e.getPosition()); 
		}
		else
		{
			m_attack->stopAttack();
		}
	}
}
